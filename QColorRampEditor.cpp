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
#include "MultiHandleSlider.hpp"
#include "ColorRampWidget.hpp"
#include "MathUtil.hpp"
#include <QDebug>

// -----------------------------------------------------------
// QColorRampEditor ------------------------------------------
// -----------------------------------------------------------
QColorRampEditor::QColorRampEditor(QWidget* parent, Qt::Orientation orientation) : QWidget(parent)
{
    this->orientation = orientation;
    horizontalLayout = new QGridLayout();
    verticalLayout = new QGridLayout();

    leftSpacer = new QSpacerItem(12, 3, QSizePolicy::Fixed, QSizePolicy::Fixed);
    rightSpacer = new QSpacerItem(12, 3, QSizePolicy::Fixed, QSizePolicy::Fixed);
    topSpacer = new QSpacerItem(3, 11, QSizePolicy::Fixed, QSizePolicy::Fixed);
    bottomSpacer = new QSpacerItem(3, 12, QSizePolicy::Fixed, QSizePolicy::Fixed);


    if (orientation==Qt::Horizontal)
        setMinimumSize(50,40);
    else
        setMinimumSize(40,50);

    setContentsMargins(0,0,0,0);
    /*
    if (orientation==Qt::Horizontal)
        setLayout(horizontalLayout);
    else
        setLayout(verticalLayout);
*/

    //layout()->setMargin(0);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setContentsMargins(0,0,0,0);

    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(0,0,0,0);

    colorRampWidget = new ColorRampWidget(this, orientation);
    colorRampWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    colorRampWidget->setContentsMargins(0,0,0,0);

    multiHandleSlider = new MultiHandleSlider(this, orientation);

    multiHandleSlider->setContentsMargins(0,0,0,0);

    horizontalLayout->addItem(leftSpacer, 0, 0);
    horizontalLayout->addWidget(colorRampWidget, 0, 1);
    horizontalLayout->addItem(rightSpacer, 0, 2);
    horizontalLayout->addWidget(multiHandleSlider, 1, 0, 1, 3);

    verticalLayout->addWidget(multiHandleSlider, 0, 0, 3, 1);
    verticalLayout->addItem(topSpacer, 0, 1);
    verticalLayout->addWidget(colorRampWidget, 1, 1, 1, 1);
    verticalLayout->addItem(bottomSpacer, 2, 1);

    // init sliders
    QVector<QPair<qreal, QColor> > ramp;
    //ramp.push_back(QPair<qreal, QColor>(0.0, Qt::black));
    ramp.push_back(QPair<qreal, QColor>(0.5, Qt::red));
    multiHandleSlider->setColorRamp(ramp);
    connect(multiHandleSlider, &MultiHandleSlider::sliderChanged, this, &QColorRampEditor::onColorRampChanged);
    connect(colorRampWidget, &ColorRampWidget::colorClicked, this, &QColorRampEditor::onColorClicked);
    connect(multiHandleSlider, &MultiHandleSlider::sliderValueChanged, this, &QColorRampEditor::onSliderValueChanged);

    connect(multiHandleSlider, &MultiHandleSlider::sliderChanged, this, &QColorRampEditor::colorRampChanged);
    connect(colorRampWidget, &ColorRampWidget::colorClicked, this, &QColorRampEditor::colorClicked);
    connect(multiHandleSlider, &MultiHandleSlider::sliderValueChanged, this, &QColorRampEditor::sliderValueChanged);
}

QColorRampEditor::~QColorRampEditor() {
}

QVector<QRgb> QColorRampEditor::getColorTable() {
    // get ramp and normalize
    QVector<QPair<qreal, QColor> > ramp = multiHandleSlider->getColorRamp();
    for (int i=0; i<ramp.size(); i++) ramp[i].first = multiHandleSlider->getNormalizedValue(ramp[i].first);
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
    //static QPoint getPositionForNormalizedValue(qreal value, qreal boundarySpace, qreal sliderHandleWidth, qreal sliderWidth, Qt::Orientation orientation);

    QPoint position = MathUtil::getPositionForNormalizedValue(value, colorRampWidget->contentsRect(), 0, Qt::Horizontal);
    multiHandleSlider->addSlider(value, color);
}

void QColorRampEditor::onSliderValueChanged(QUuid sliderId, qreal value)
{
    emit sliderValueChanged(sliderId, value);
}

void QColorRampEditor::onColorRampChanged()
{
    qDebug()<<"Slider changed";
    ColorRamp colorRamp = multiHandleSlider->getColorRamp();
    colorRampWidget->setColorRamp(colorRamp);
    colorRampWidget->update();
}

void QColorRampEditor::resizeEvent (QResizeEvent*) {
}
/*
void QColorRampEditor::mousePressEvent(QMouseEvent* e) {
    if (e->button()== Qt::LeftButton)
    {
        QRect crec = contentsRect();
        //crec.adjust(bspace_,0,-bspace_,0);
        if (crec.contains(e->pos(), true )) // test mouse is in ramp
        {
            multiHandleSlider->addSlider(e->pos(), Qt::white);
        }
    }
}
*/

bool QColorRampEditor::isVertical() const
{
    return orientation == Qt::Orientation::Vertical;
}

void QColorRampEditor::setVertical(const bool &vertical)
{
    orientation = vertical ? Qt::Orientation::Vertical : Qt::Orientation::Horizontal;
    setLayout(vertical ? verticalLayout : horizontalLayout);
    multiHandleSlider->setVertical(vertical);
    colorRampWidget->setVertical(vertical);
    if (orientation==Qt::Horizontal)
    {
        multiHandleSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        multiHandleSlider->setFixedHeight(16);
    }
    else
    {
        multiHandleSlider->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        multiHandleSlider->setFixedWidth(16);
    }
}
