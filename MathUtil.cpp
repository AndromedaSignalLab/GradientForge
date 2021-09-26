#include "MathUtil.hpp"

MathUtil::MathUtil()
{
}

qreal getNormalizedValue(qreal value, qreal min, qreal max) {
    return (value - min)/(max-min);
}

qreal getNormalizedValue(qreal value, qreal min, qreal max, qreal boundarySpace, qreal sliderHandleWidth, qreal sliderWidth, qreal sliderHeight, Qt::Orientation orientation) {
    return (value - min)/(max-min);
}

qreal getNormalizedValue(QPoint position, QRect contentsRect, qreal sliderHandleWidth, Qt::Orientation orientation) {
    int boundarySpace = sliderHandleWidth/2;

    qreal normalizedValue = orientation == Qt::Horizontal ? 1.0*(position.x()-boundarySpace)/(contentsRect.width() - boundarySpace*2) : 1.0*(position.y()-boundarySpace)/(contentsRect.height() - boundarySpace*2);
    return normalizedValue;
}


QPoint getPosition(qreal normalizedValue, qreal boundarySpace, qreal sliderHandleWidth,  qreal sliderWidth, Qt::Orientation orientation)
{
    QPoint position;
    qreal pos;
    pos = normalizedValue*sliderWidth;
    pos -= sliderHandleWidth/2;
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

