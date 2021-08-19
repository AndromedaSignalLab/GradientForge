#include "MathUtil.hpp"

MathUtil::MathUtil()
{
}

qreal MathUtil::getNormalizedValue(qreal value, qreal min, qreal max) {
    return (value - min)/(max-min);
}


QPoint MathUtil::getPositionForNormalizedValue(qreal value, qreal boundarySpace, qreal sliderHandleWidth, qreal sliderHandleHeight,  qreal sliderWidth, qreal sliderHeight, Qt::Orientation orientation)
{
    QPoint position;
    qreal pos;
    if (orientation==Qt::Horizontal) {
        position.setY(0);
        pos = value*sliderWidth;
        pos -= sliderHandleWidth/2;
        pos += boundarySpace;
        position.setX(pos);
    }
    else
    {
        position.setX(0);
        pos = value*sliderHeight;
        pos -= sliderHandleHeight/2;
        pos += boundarySpace;
        position.setY(pos);
    }

    return position;
}

