#include "MathUtil.hpp"

MathUtil::MathUtil()
{
}

qreal MathUtil::getNormalizedValue(qreal value, qreal min, qreal max) {
    return (value - min)/(max-min);
}

qreal MathUtil::getNormalizedValue(qreal value, qreal min, qreal max, qreal boundarySpace, qreal sliderHandleWidth, qreal sliderWidth, qreal sliderHeight, Qt::Orientation orientation) {
    return (value - min)/(max-min);
}

qreal MathUtil::getNormalizedValue(QPoint position, QRect contentsRect, qreal sliderHandleWidth, Qt::Orientation orientation) {
    int boundarySpace = sliderHandleWidth/2;

    qreal normalizedValue = orientation == Qt::Horizontal ? 1.0*(position.x() + 1 - boundarySpace)/(contentsRect.width() - boundarySpace*2) : 1.0*(position.y() + 1 -boundarySpace)/(contentsRect.height() - boundarySpace*2);
    return normalizedValue;
}

QPoint MathUtil::getPositionForNormalizedValue(qreal normalizedValue, QRect contentsRect, qreal boundarySpace, qreal sliderHandleWidth, Qt::Orientation orientation)
{
    QPoint position;
    qreal pos;
    if (orientation==Qt::Horizontal) {
        position.setY(0);
        contentsRect.adjust(boundarySpace,0,-boundarySpace,0);
        pos = (normalizedValue)*contentsRect.width();
        //pos -= sliderWidth;
        pos += boundarySpace;
        position.setX(pos);
    }
    else
    {
        position.setX(0);
        contentsRect.adjust(0, boundarySpace,0,-boundarySpace);
        pos = (normalizedValue)*contentsRect.height();
        //pos -= sliderHeight/2;
        pos += boundarySpace;
        position.setY(pos);
    }

    return position;

    /*
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
*/
}

QPoint MathUtil::getPositionForNormalizedValue(qreal normalizedValue, qreal boundarySpace, qreal sliderHandleWidth,  qreal sliderWidth, Qt::Orientation orientation)
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

