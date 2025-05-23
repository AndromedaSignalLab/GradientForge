/*
ColorRampWidget class definitions
Belongs to GradientEditor Project
Copyright (C) 2021 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "ColorRampWidget.hpp"

#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <algorithm>
#include "GradientUtil.hpp"
#include "MathUtil.hpp"

ColorRampWidget::ColorRampWidget(QWidget* parent, Qt::Orientation orientation) : QWidget(parent) {
    this->orientation = orientation;
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    setContentsMargins(0,0,0,0);
}

QColor ColorRampWidget::getColor(qreal key) const
{
    return GradientUtil::getColor(key, colorRamp);
}

void ColorRampWidget::onColorRampChanged()
{
    //this->colorRamp = colorRamp;
    //update();
}

void ColorRampWidget::mousePressEvent(QMouseEvent* e) {
    if (e->button()== Qt::LeftButton)
    {
        QRect crec = contentsRect();
        //crec.adjust(bspace_,0,-bspace_,0);
        QColor colorJustClicked;
        qreal normalizedValue;
        if (crec.contains(e->pos(), false )) // test mouse is in ramp
        {
            /*
            if(orientation == Qt::Horizontal) {
                crec.adjust(boundarySpace-1, 0, -boundarySpace+1, 0);
            }
            else {
                crec.adjust(0, boundarySpace-1, 0, -boundarySpace+1);
            }*/
            normalizedValue = GradientEditor::MathUtil::getNormalizedValue(e->pos(), crec, 0, orientation);
            colorJustClicked = GradientUtil::getColor(normalizedValue, colorRamp);
            emit colorClicked(normalizedValue, colorJustClicked);
        }
    }
}

const QGradientStops & ColorRampWidget::getcolorRamp() const
{
    return colorRamp;
}

void ColorRampWidget::setColorRamp(const QGradientStops &colorRamp)
{
    this->colorRamp = colorRamp;
}

void ColorRampWidget::paintEvent(QPaintEvent* e) {
    QPainter painter(this);
    painter.setPen(Qt::black);

    QLinearGradient grad;
    QRect crec = contentsRect();
    if (orientation==Qt::Horizontal)
    {
        //crec.adjust(rampeditor_->slidewid_->getBoundSpace(),0,-rampeditor_->slidewid_->getBoundSpace(),-1);
        //crec.adjust(boundarySpace-1, 0, -boundarySpace+1, 0);
        grad = QLinearGradient( 0, 0, crec.width()-1, 0);
    }
    else
    {
        //crec.adjust(0,rampeditor_->slidewid_->getBoundSpace(),-1,-rampeditor_->slidewid_->getBoundSpace());
        //crec.adjust(0, boundarySpace-1, 0, -boundarySpace+1);
        grad = QLinearGradient( 0, 0, 0, crec.height()-1);
    }

    if (orientation==Qt::Horizontal)
        for (int i=0; i<colorRamp.size(); i++) {
            //qreal nval = rampeditor_->slidewid_->getNormalizedValue(colorRamp[i].first);
            grad.setColorAt(colorRamp[i].first, colorRamp[i].second);
        }
    else
        for (int i=0; i<colorRamp.size(); i++) {
            //qreal nval = rampeditor_->slidewid_->getNormalizedValue(colorRamp[i].first);
            grad.setColorAt(1.0 - colorRamp[i].first, colorRamp[i].second);
        }


    /*for (int i=0; i<rampeditor_->sliders.size(); i++)
    {
        qreal nval = (rampeditor_->sliders[i]->value - rampeditor_->mi_)/(rampeditor_->ma_-rampeditor_->mi_);
        grad.setColorAt(nval, rampeditor_->sliders[i]->getColor());
    }*/

    painter.fillRect( crec, grad);
    //painter.drawRect(crec);

    QWidget::paintEvent(e);
}

bool ColorRampWidget::isVertical() const
{
    return orientation == Qt::Orientation::Vertical;
}

void ColorRampWidget::setVertical(const bool &vertical)
{
    orientation = vertical ? Qt::Orientation::Vertical : Qt::Orientation::Horizontal;
}
