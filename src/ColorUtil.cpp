#include "ColorUtil.hpp"

#include <QPropertyAnimation>

QColor ColorUtil::getColor(qreal key, ColorRampMap colorRamp) {
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

QColor ColorUtil::getColor(qreal key, ColorRamp colorRamp) {
    // key must belong to [0,1]
    //key = Clip(key, 0.0, 1.0) ;
    key = std::clamp(key, 0.0, 1.0);

    // directly get color if known
    for(ColorRampElement &colorRampElement : colorRamp) {
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
    for(ColorRampElement &colorRampElement : colorRamp) {
        interpolator.setKeyValueAt(colorRampElement.first, colorRampElement.second) ;
    }

    interpolator.setCurrentTime(key*granularite) ;
    return interpolator.currentValue().value<QColor>();
}

