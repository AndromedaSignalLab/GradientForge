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

    rampwid_ = new QRampWidget();
    rampwid_->rampeditor_ = this;
    rampwid_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    rampwid_->setContentsMargins(0,0,0,0);

    layout()->addWidget(rampwid_);

    slidewid_ = new QSlidersWidget(orientation);
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
    ramp.push_back(QPair<qreal, QColor>(0.0, Qt::black));
    ramp.push_back(QPair<qreal, QColor>(1.0, Qt::red));
    slidewid_->setRamp(ramp);
    connect(slidewid_, &QSlidersWidget::colorRampChanged, rampwid_, &QRampWidget::onColorRampChanged);
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

// -----------------------------------------------------------
// QRampWidget -----------------------------------------------
// -----------------------------------------------------------

QRampWidget::QRampWidget(QWidget* parent) : QWidget(parent) {
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    setContentsMargins(0,0,0,0);
    setMinimumHeight(5);
    setMinimumWidth(5);
}

void QRampWidget::onColorRampChanged(ColorRamp colorRamp)
{
    this->colorRamp = colorRamp;
    update();
}

void QRampWidget::paintEvent(QPaintEvent* e) {
    QPainter painter(this);
    painter.setPen(Qt::black);

    QLinearGradient grad;
    QRect crec = contentsRect();
    if (rampeditor_->orientation==Qt::Horizontal)
    {
        crec.adjust(rampeditor_->slidewid_->getBoundSpace(),0,-rampeditor_->slidewid_->getBoundSpace(),-1);
        grad = QLinearGradient( 0, 0, crec.width()-1, 0);
    }
    else
    {
        crec.adjust(0,rampeditor_->slidewid_->getBoundSpace(),-1,-rampeditor_->slidewid_->getBoundSpace());
        grad = QLinearGradient( 0, 0, 0, crec.height()-1);
    }


    auto ramp = rampeditor_->slidewid_->getRamp();

    for (int i=0; i<ramp.size(); i++)
    {
        qreal nval = rampeditor_->slidewid_->getNormalizedValue(ramp[i].first);
        grad.setColorAt(nval, ramp[i].second);
    }

    /*for (int i=0; i<rampeditor_->sliders.size(); i++)
    {
        qreal nval = (rampeditor_->sliders[i]->value - rampeditor_->mi_)/(rampeditor_->ma_-rampeditor_->mi_);
        grad.setColorAt(nval, rampeditor_->sliders[i]->getColor());
    }*/

    painter.fillRect( crec, grad);
    painter.drawRect(crec);

    QWidget::paintEvent(e);
}
