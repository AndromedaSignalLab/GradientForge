#include "SliderHandle.hpp"
#include <QPainter>
#include <QPolygon>
#include "MathUtil.hpp"
#include <cmath>

SliderHandle::SliderHandle(const SliderHandleProperties &properties, QWidget* parent) : QWidget(parent)
{
    this->properties = properties;
    this->parent = parent;
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
    QRect crec = parent->contentsRect();
    return MathUtil::getNormalizedValue(pos(), crec, properties.width/2, properties.orientation);
}

void SliderHandle::setValue(const qreal & value)
{
    QRect crec = parent->contentsRect();
    QPoint pos = MathUtil::getPositionForNormalizedValue(value, crec, getBoundarySpace(), properties.orientation);
    move(pos);
}

void SliderHandle::update()
{
    /*
    QRect crec = parent->contentsRect();
    qreal pos;
    int boundarySpace = properties.width/2;
    if (properties.orientation==Qt::Horizontal)
    {
        crec.adjust(boundarySpace,0,-boundarySpace,0);
        pos = getValue()*crec.width();
        //pos -= getBoundarySpace();
        pos += boundarySpace;
        move(pos,0);
    }
    else
    {
        crec.adjust(0, boundarySpace,0,-boundarySpace);
        pos = getValue()*crec.height();
        //pos -= getBoundarySpace();
        pos += boundarySpace;
        move(0,pos);
    }*/
}

void SliderHandle::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.setBrush(properties.color);
    if (properties.orientation==Qt::Horizontal) {
        //QRect rec(0,7,8,8);
        //painter.drawRect(rec);
        QPolygon pp;
        //7 = height - capHeight
        //15 = height
        //8 = width
        //4 = width/2
        //(0.00,7.00), (4.00,0.00), (8.00,7.00), (8.00,15.00), (0.00,15.00)
        //--> (4,0), (8,7), (8,15), (0,15), (0,7)
        pp << QPoint(properties.width/2, 0) << QPoint(properties.width, properties.capHeight) << QPoint(properties.width, properties.height) << QPoint(0, properties.height) << QPoint(0, properties.capHeight);
//        pp<<QPoint(0,7)<<QPoint(4,4)<<QPoint(8,7)<<QPoint(8,15)<<QPoint(0,15);
        //pp<<QPoint(7,0)<<QPoint(4,4)<<QPoint(7,8)<<QPoint(15,8)<<QPoint(15,0);

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
        pp<<QPoint(0,0)<<QPoint(properties.height - properties.capHeight, 0) << QPoint(properties.height, properties.width/2)<<QPoint(properties.height - properties.capHeight, properties.width) <<QPoint(0,properties.width);
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
