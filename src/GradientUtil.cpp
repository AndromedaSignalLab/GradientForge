/*
GradientUtil utilization class definitions
Belongs to GradientEditor Project
Copyright (C) 2021 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "GradientUtil.hpp"

#include <QPropertyAnimation>

QColor GradientUtil::getColor(qreal key, const ColorRampMap &colorRamp) {
    // key must belong to [0,1]
    //key = Clip(key, 0.0, 1.0) ;
    key = std::clamp(key, 0.0, 1.0);

    // directly get color if known
    if(colorRamp.contains(key))
    {
        return colorRamp.value(key) ;
    }

    // else, emulate a linear gradient
    QPropertyAnimation interpolator ;
    const qreal granularite = 100.0 ;
    interpolator.setEasingCurve(QEasingCurve::Linear) ;
    interpolator.setDuration(granularite) ;
    foreach( qreal key, colorRamp.keys() )
    {
        interpolator.setKeyValueAt(key, colorRamp.value(key)) ;
    }
    interpolator.setCurrentTime(key*granularite) ;
    return interpolator.currentValue().value<QColor>() ;
}

QColor GradientUtil::getColor(qreal key, const QGradientStops &colorRamp) {
    // key must belong to [0,1]
    //key = Clip(key, 0.0, 1.0) ;
    key = std::clamp(key, 0.0, 1.0);

    // directly get color if known
    for(const QGradientStop &colorRampElement : colorRamp) {
        if(colorRampElement.first == key)
            return colorRampElement.second;
    }

    if(colorRamp.isEmpty())
        return Qt::black;

    if(key < colorRamp[0].first)
        return colorRamp[0].second;

    if(key > colorRamp.last().first)
        return colorRamp.last().second;

    // else, emulate a linear gradient
    QPropertyAnimation interpolator ;
    const qreal granularite = 100.0 ;
    interpolator.setEasingCurve(QEasingCurve::Linear) ;
    interpolator.setDuration(granularite) ;
    for(const QGradientStop &colorRampElement : colorRamp) {
        interpolator.setKeyValueAt(colorRampElement.first, colorRampElement.second) ;
    }

    interpolator.setCurrentTime(key*granularite) ;
    return interpolator.currentValue().value<QColor>();
}
