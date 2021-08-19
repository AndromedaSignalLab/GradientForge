#pragma once

#include <QPoint>
#include <QRect>
#include <QtGlobal>

class MathUtil
{
public:
    MathUtil();
    static qreal getNormalizedValue(qreal value, qreal min, qreal max);
    static QPoint getPositionForNormalizedValue(qreal value, qreal boundarySpace, qreal sliderHandleWidth, qreal sliderHandleHeight, qreal sliderWidth, qreal sliderHeight, Qt::Orientation orientation);
};

