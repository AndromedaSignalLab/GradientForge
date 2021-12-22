/*
SliderHandle class definitions
Belongs to GradientEditor Project
Copyright (C) 2021 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "SliderHandle.hpp"
#include <QPainter>
#include <QPolygon>
#include "MathUtil.hpp"
#include <cmath>

SliderHandle::SliderHandle(const SliderHandleProperties &properties, QWidget* parent) : QWidget(parent)
{
    this->properties = properties;
    this->parent = parent;
    this->value = 0;
    init();
}

void SliderHandle::init()
{
    if (properties.orientation==Qt::Horizontal)
        setFixedSize(properties.width, properties.height);
    else
        setFixedSize(properties.height, properties.width);
}

void SliderHandle::setColor(QColor col)
{
    properties.color = col;
}

QColor SliderHandle::getColor() const
{
    return properties.color;
}

int SliderHandle::getBoundarySpace()
{
    return std::ceil(double(properties.width)/2);
}

qreal SliderHandle::getValue() const
{
    return value;
}

void SliderHandle::setValue(const qreal & value)
{
    this->value = value;
}

void SliderHandle::setOrientation(const Qt::Orientation & orientation)
{
    properties.orientation = orientation;
    init();
}

Qt::Orientation SliderHandle::getOrientation() const
{
    return properties.orientation;
}

void SliderHandle::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::RenderHint::Antialiasing|QPainter::RenderHint::SmoothPixmapTransform);
    painter.setPen(Qt::black | Qt::SolidLine);
    painter.setBrush(properties.color);
    if (properties.orientation==Qt::Horizontal) {
        /*
           handleProperties.width = 24;
           handleProperties.capHeight = 8;
           handleProperties.height = 16;
         */
        //QRect rec(0,7,8,8);
        //painter.drawRect(rec);
        QPolygon pp;
        //7 = height - capHeight
        //15 = height
        //8 = width
        //4 = width/2
        //(0.00,7.00), (4.00,0.00), (8.00,7.00), (8.00,15.00), (0.00,15.00)
        //--> (4,0), (8,7), (8,15), (0,15), (0,7)
        pp << QPoint((properties.width/2) -1, 0) << QPoint(properties.width-1, properties.capHeight-1) << QPoint(properties.width-1, properties.height-1) << QPoint(0, properties.height-1) << QPoint(0, properties.capHeight-1);
//        pp<<QPoint(0,7)<<QPoint(4,4)<<QPoint(8,7)<<QPoint(8,15)<<QPoint(0,15);
        //pp<<QPoint(7,0)<<QPoint(4,4)<<QPoint(7,8)<<QPoint(15,8)<<QPoint(15,0);

        //painter.drawLines(pp);
        painter.drawPolygon(pp, Qt::OddEvenFill);
    }
    else
    {
        //QRect rec(7,0,8,8);
        //painter.drawRect(rec);
        QPolygon pp;
        //8 = capheight
        //7 = height - capHeight = properties.height - properties.capHeight - 1
        //15 = height = properties.height - 1
        //8 = width = properties.width - 1
        //4 = width/2 = properties.width/2
        //(8.00,0.00), (15.00,4.00), (8.00,8.00), (0.00,8.00), (0.00,0.00)

        //pp<<QPoint(7,0)<<QPoint(4,4)<<QPoint(7,8)<<QPoint(15,8)<<QPoint(15,0);


        //Right to left
        //pp << QPoint(properties.height - properties.capHeight - 1, 0) << QPoint(0, properties.width/2) << QPoint(properties.height - properties.capHeight - 1, properties.width - 1) << QPoint(properties.height - 1, properties.width - 1) << QPoint(properties.height - 1, 0);
        //Left to right
        //           (7,0)                                                                 (15,4)                                                    (7,8)                                       (0,8)                     (0,0)
        //pp << QPoint(properties.height - properties.capHeight - 1, 0) << QPoint(properties.height - 1, properties.width/2) << QPoint(properties.height - properties.capHeight - 1, properties.width - 1) << QPoint(0, properties.width - 1) << QPoint(0, 0);
        //pp<<QPoint(7.0,0.0) << QPoint(15.0,4.0) << QPoint(7.0,8.0) << QPoint(0.0,8.0) << QPoint(0.0,0.0);
        //(0,0) (height-capheight,0) (height, width/2) (height-capheight, width) (0, width)
        //pp<<QPoint(0,0)<<QPoint(7,0) << QPoint(15, 4) << QPoint(7, 8) <<QPoint(0, 8);
        pp<<QPoint(0,0)<<QPoint(properties.height - properties.capHeight - 1, 0) << QPoint(properties.height -1, (properties.width/2) - 1)<<QPoint(properties.height - properties.capHeight - 1, properties.width - 1) <<QPoint(0,properties.width - 1);
        //pp<<QPoint(0,0)<<QPoint(properties.height - properties.capHeight, 0) << QPoint(properties.height, properties.width/2)<<QPoint(properties.height - properties.capHeight, properties.width) <<QPoint(0,properties.width);
        painter.drawPolygon(pp, Qt::OddEvenFill);
    }

}

qreal SliderHandle::calculateNominalValueFromPosition()
{
    int boundarySpace = this->properties.width/2;
    QRect crec = parent->contentsRect();

    qreal position = properties.orientation == Qt::Horizontal ? 1.0*(pos().x()-boundarySpace)/(crec.width() - boundarySpace*2) : 1.0*(pos().y()-boundarySpace)/(crec.height() - boundarySpace*2);
    return position;
}
