#include "SliderHandle.hpp"
#include <QPainter>
#include <QPolygon>

SliderHandle::SliderHandle(Qt::Orientation orientation, QColor col, QWidget* parent) : QWidget(parent),
    color(col)
{
    this->orientation = orientation;
    if (orientation==Qt::Horizontal)
        setFixedSize(9, 16);
    else
        setFixedSize(16,9);
}

void SliderHandle::setColor(QColor col)
{
    color = col;
}

QColor SliderHandle::getColor()
{
    return color;
}

void SliderHandle::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.setBrush(color);
    if (orientation==Qt::Horizontal)
    {
        //QRect rec(0,7,8,8);
        //painter.drawRect(rec);
        QPolygon pp;
        pp << QPoint(0,7) << QPoint(4,0) << QPoint(8,7) << QPoint(8, 15) << QPoint(0, 15);
        painter.drawPolygon(pp, Qt::OddEvenFill);
    }
    else
    {
        //QRect rec(7,0,8,8);
        //painter.drawRect(rec);
        QPolygon pp;
        pp << QPoint(7,0) << QPoint(0,4) << QPoint(7,8) << QPoint(15, 8) << QPoint(15,0);
        painter.drawPolygon(pp, Qt::OddEvenFill);
    }

}
