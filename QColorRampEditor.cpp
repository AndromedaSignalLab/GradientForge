/****************************************************************************
**
** Copyright (c) 2012 Richard Steffen and/or its subsidiary(-ies).
** All rights reserved.
** Contact: rsteffen@messbild.de, rsteffen@uni-bonn.de
**
** QColorRampEditor is free to use unter the terms of the LGPL 2.1 License in
** Free and Commercial Products.
****************************************************************************/

#include "QColorRampEditor.hpp"
#include <QPainter>
#include <QMouseEvent>
#include "QSlidersWidget.hpp"
#include "ColorRampWidget.hpp"
#include "MathUtil.hpp"
#include <QDebug>

// -----------------------------------------------------------
// QColorRampEditor ------------------------------------------
// -----------------------------------------------------------
QColorRampEditor::QColorRampEditor(QWidget* parent, Qt::Orientation orientation) : QWidget(parent)
{
    this->orientation = orientation;
    if (orientation==Qt::Horizontal)
        setMinimumSize(50,40);
    else
        setMinimumSize(40,50);

    setContentsMargins(0,0,0,0);
    if (orientation==Qt::Horizontal)
        setLayout(new QVBoxLayout());
    else
        setLayout(new QHBoxLayout());
    //layout()->setMargin(0);
    layout()->setSpacing(0);
    layout()->setContentsMargins(0,0,0,0);

    rampwid_ = new ColorRampWidget();
    rampwid_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    rampwid_->setContentsMargins(0,0,0,0);

    layout()->addWidget(rampwid_);

    slidewid_ = new MultiHandleSliderWidget(orientation);
    if (orientation==Qt::Horizontal)
    {
        slidewid_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        slidewid_->setFixedHeight(16);
    }
    else
    {
        slidewid_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        slidewid_->setFixedWidth(16);
    }
    slidewid_->setContentsMargins(0,0,0,0);
    layout()->addWidget(slidewid_);

    // init sliders
    QVector<QPair<qreal, QColor> > ramp;
    //ramp.push_back(QPair<qreal, QColor>(0.0, Qt::black));
    ramp.push_back(QPair<qreal, QColor>(0.5, Qt::red));
    slidewid_->setRamp(ramp);
    connect(slidewid_, &MultiHandleSliderWidget::colorRampChanged, rampwid_, &ColorRampWidget::onColorRampChanged);
    connect(rampwid_, &ColorRampWidget::colorClicked, this, &QColorRampEditor::onColorClicked);
}

QColorRampEditor::~QColorRampEditor() {
}

QVector<QRgb> QColorRampEditor::getColorTable() {
    // get ramp and normalize
    QVector<QPair<qreal, QColor> > ramp = slidewid_->getRamp();
    for (int i=0; i<ramp.size(); i++) ramp[i].first = slidewid_->getNormalizedValue(ramp[i].first);
    QVector<QRgb> ctable;
    int index = 0;
    for (int i = 0; i < 256; i++)
    {
        float val = 1.0f*i/255;
        if (val>ramp[index+1].first) index++;
        // linear interpolate color
        QColor cc;
        float d = val - ramp[index].first;
        float dr = ramp[index+1].second.redF() - ramp[index].second.redF();
        float dg = ramp[index+1].second.greenF() - ramp[index].second.greenF();
        float db = ramp[index+1].second.blueF() - ramp[index].second.blueF();
        qreal dd = ramp[index+1].first - ramp[index].first;
        float red = ramp[index].second.redF() +  d*dr/dd;
        float green = ramp[index].second.greenF() +  d*dg/dd;
        float blue = ramp[index].second.blueF() +  d*db/dd;
		if (red<0.0f) red=0;
		if (red>1.0f) red=1;
		if (green<0.0f) green=0;
		if (green>1.0f) green=1;
		if (blue<0.0f) blue=0;
		if (blue>1.0f) blue=1;
        cc.setRedF(red);
        cc.setGreenF(green);
        cc.setBlueF(blue);
        ctable.push_back(cc.rgb());
    }
    return ctable;
}

void QColorRampEditor::onColorClicked(double value, QColor color)
{
    qDebug()<<"Color clicked. Value: "<< value << " Color: " << color;
    QPoint position = MathUtil::getPositionForNormalizedValue(value, 5, 8, 6, 421, 31, Qt::Horizontal);
    slidewid_->addSlider(position, color);
}

void QColorRampEditor::resizeEvent (QResizeEvent*) {
}

void QColorRampEditor::mousePressEvent(QMouseEvent* e) {
    if (e->button()== Qt::LeftButton)
    {
        QRect crec = contentsRect();
        //crec.adjust(bspace_,0,-bspace_,0);
        if (crec.contains(e->pos(), true )) // test mouse is in ramp
        {
            slidewid_->addSlider(e->pos(), Qt::white);
        }
    }
}
