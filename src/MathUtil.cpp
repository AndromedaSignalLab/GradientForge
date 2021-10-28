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
