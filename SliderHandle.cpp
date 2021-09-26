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
    return value;
}

void SliderHandle::setValue(qreal value) {
    this->value = value;
    update();
    return;
}

void SliderHandle::update()
{
    QRect crec = parent->contentsRect();
    qreal pos;
    int boundarySpace = properties.width/2;
    if (properties.orientation==Qt::Horizontal)
    {
        crec.adjust(boundarySpace,0,-boundarySpace,0);
        pos = value*crec.width();
        //pos -= getBoundarySpace();
        pos += boundarySpace;
        move(pos,0);
    }
    else
    {
        crec.adjust(0, boundarySpace,0,-boundarySpace);
        pos = value*crec.height();
        //pos -= getBoundarySpace();
        pos += boundarySpace;
        move(0,pos);
    }
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
        pp << QPoint(0, properties.height - properties.capHeight - 1) << QPoint(properties.width/2, 0) << QPoint(properties.width - 1, properties.height - properties.capHeight - 1) << QPoint(properties.width - 1, properties.height - 1) << QPoint(0, properties.height - 1);
        painter.drawPolygon(pp, Qt::OddEvenFill);
    }
    else
    {
        //QRect rec(7,0,8,8);
        //painter.drawRect(rec);
        QPolygon pp;
        pp << QPoint(properties.height - properties.capHeight - 1, 0) << QPoint(0, properties.width/2) << QPoint(properties.height - properties.capHeight - 1, properties.width - 1) << QPoint(properties.height - 1, properties.width - 1) << QPoint(properties.height - 1, 0);
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
