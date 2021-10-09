#pragma once

#include <QPoint>
#include <QRect>
#include <QtGlobal>

class MathUtil
{
public:
    MathUtil();
    static qreal getNormalizedValue(qreal value, qreal min, qreal max);
    static qreal getNormalizedValue(qreal value, qreal min, qreal max, qreal boundarySpace, qreal sliderHandleWidth, qreal sliderWidth, qreal sliderHeight, Qt::Orientation orientation);
    static qreal getNormalizedValue(QPoint position, QRect contentsRect, qreal boundarySpace, Qt::Orientation orientation);
    static QPoint getPositionForNormalizedValue(qreal value, qreal boundarySpace, qreal sliderHandleWidth, qreal sliderWidth, Qt::Orientation orientation);
};

