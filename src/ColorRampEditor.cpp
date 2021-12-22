/*
ColorRampEditor class definitions
Belongs to GradientEditor Project
Copyright (C) 2021 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#include "ColorRampEditor.hpp"
#include <QPainter>
#include <QMouseEvent>
#include "MultiHandleSlider.hpp"
#include "ColorRampWidget.hpp"
#include "MathUtil.hpp"
#include <QDebug>

ColorRampEditor::ColorRampEditor(QWidget* parent, Qt::Orientation orientation) : QWidget(parent)
{
    this->orientation = orientation;
    horizontalLayout = new QGridLayout();
    verticalLayout = new QGridLayout();

    leftSpacer = new QSpacerItem(11, 3, QSizePolicy::Fixed, QSizePolicy::Fixed);
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
    connect(multiHandleSlider, &MultiHandleSlider::sliderChanged, this, &ColorRampEditor::onColorRampChanged);
    connect(colorRampWidget, &ColorRampWidget::colorClicked, this, &ColorRampEditor::onColorClicked);
    connect(multiHandleSlider, &MultiHandleSlider::sliderValueChanged, this, &ColorRampEditor::onSliderValueChanged);

    connect(multiHandleSlider, &MultiHandleSlider::sliderChanged, this, &ColorRampEditor::colorRampChanged);
    connect(colorRampWidget, &ColorRampWidget::colorClicked, this, &ColorRampEditor::colorClicked);
    connect(multiHandleSlider, &MultiHandleSlider::sliderValueChanged, this, &ColorRampEditor::sliderValueChanged);
    updateColorRamp();
}

ColorRampEditor::~ColorRampEditor() {
}

QVector<QRgb> ColorRampEditor::getColorTable() {
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

void ColorRampEditor::onColorClicked(double value, QColor color)
{
    qDebug()<<"Color clicked. Value: "<< value << " Color: " << color;
    //static QPoint getPositionForNormalizedValue(qreal value, qreal boundarySpace, qreal sliderHandleWidth, qreal sliderWidth, Qt::Orientation orientation);

    QPoint position = MathUtil::getPositionForNormalizedValue(value, colorRampWidget->contentsRect(), 0, Qt::Horizontal);
    multiHandleSlider->addSlider(value, color);
}

void ColorRampEditor::onSliderValueChanged(QUuid sliderId, qreal value)
{
    emit sliderValueChanged(sliderId, value);
}

void ColorRampEditor::onColorRampChanged()
{
    qDebug()<<"Slider changed";
    updateColorRamp();
}

void ColorRampEditor::resizeEvent (QResizeEvent*) {
}

void ColorRampEditor::changeLayout(QLayout * newLayout)
{
    QLayout *oldLayout = layout();

    if(oldLayout != nullptr){
        oldLayout->deleteLater();
    }

    setLayout(newLayout);

}

void ColorRampEditor::updateColorRamp()
{
    ColorRamp colorRamp = multiHandleSlider->getColorRamp();
    colorRampWidget->setColorRamp(colorRamp);
    colorRampWidget->update();
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

bool ColorRampEditor::isVertical() const
{
    return orientation == Qt::Orientation::Vertical;
}

void ColorRampEditor::setVertical(const bool &vertical)
{
    orientation = vertical ? Qt::Orientation::Vertical : Qt::Orientation::Horizontal;
    changeLayout(vertical ? verticalLayout : horizontalLayout);
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

ColorRamp ColorRampEditor::getColorRamp()
{
    return multiHandleSlider->getColorRamp();
}

void ColorRampEditor::setColorRamp(const ColorRamp & colorRamp)
{
    multiHandleSlider->setColorRamp(colorRamp);
}
