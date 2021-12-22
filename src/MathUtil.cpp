/*
MathUtil utilization class definitions
Belongs to GradientEditor Project
Copyright (C) 2021 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "MathUtil.hpp"

MathUtil::MathUtil()
{
}

qreal MathUtil::getNormalizedValue(qreal value, qreal min, qreal max) {
    return (value - min)/(max-min);
}

qreal MathUtil::getNormalizedValue(QPoint position, QRect contentsRect, qreal boundarySpace, Qt::Orientation orientation) {
    qreal normalizedValue;
    if (orientation==Qt::Horizontal) {
        contentsRect.adjust(boundarySpace,0,-boundarySpace,0);
        normalizedValue = (1.0*(position.x() - contentsRect.x()))/(contentsRect.width());
    }
    else {
        contentsRect.adjust(0, boundarySpace,0,-boundarySpace);
        normalizedValue = 1 - (1.0*(position.y() - contentsRect.y()))/(contentsRect.height());
    }
    return normalizedValue;
}

QPoint MathUtil::getPositionForNormalizedValue(qreal normalizedValue, QRect contentsRect, qreal boundarySpace, Qt::Orientation orientation)
{
    QPoint position;
    qreal pos;
    if (orientation==Qt::Horizontal) {
        position.setY(contentsRect.y());
        contentsRect.adjust(boundarySpace,0,-boundarySpace,0);
        pos = (normalizedValue)*contentsRect.width();
        pos += boundarySpace;
        position.setX(pos);
        position.setY(contentsRect.y());
    }
    else
    {
        position.setX(contentsRect.x());
        contentsRect.adjust(0, boundarySpace,0,-boundarySpace);
        pos = (1-normalizedValue)*contentsRect.height();
        pos += boundarySpace;
        position.setX(contentsRect.x());
        position.setY(pos);
    }

    return position;
}

/*
QPoint MathUtil::getPositionForNormalizedValue(qreal normalizedValue, qreal boundarySpace,  qreal sliderWidth, Qt::Orientation orientation)
{
    QPoint position;
    qreal pos;
    pos = normalizedValue*(sliderWidth - boundarySpace*2.0);
    pos += boundarySpace;
    if (orientation==Qt::Horizontal) {
        position.setY(0);
        position.setX(pos);
    }
    else
    {
        position.setX(0);
        position.setY(pos);
    }

    return position;
}
*/
