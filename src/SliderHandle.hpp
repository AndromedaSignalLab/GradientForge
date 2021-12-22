/*
SliderHandle class declarations
Belongs to GradientEditor Project
Copyright (C) 2021 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <QPoint>
#include "MathUtil.hpp"
#include <QUuid>

struct SliderHandleProperties {
    Qt::Orientation orientation = Qt::Horizontal;
    QColor color = Qt::black;
    int width;
    int height;
    int capHeight;
};

class SliderHandle : public QWidget
{
    Q_OBJECT
public:

    /// Constructor
    SliderHandle(const SliderHandleProperties &properties, QWidget* parent=0);
    void init();

    const QUuid id = QUuid::createUuid();
    /// set the color of the slider
    void setColor(QColor color);

    /// get the color
    QColor getColor() const;
    int getBoundarySpace();

    qreal getValue() const;
    void setValue(const qreal &value);

    inline void move(int ax, int ay);
    inline void move(const QPoint &position);

    void setOrientation(const Qt::Orientation &orientation);
    Qt::Orientation getOrientation() const;

protected slots:

    /// paint the widget
    virtual void paintEvent(QPaintEvent* event);

protected:
    SliderHandleProperties properties;
    QWidget *parent;
    qreal calculateNominalValueFromPosition();
    qreal value;
};

inline void SliderHandle::move(int ax, int ay)
{
    if (properties.orientation==Qt::Horizontal) {
        //value = MathUtil::getNormalizedValue(ax, )
        QWidget::move(ax - properties.width/2, ay);
    }
    else {
        QWidget::move(ax, ay - properties.width/2);
    }
}

inline void SliderHandle::move(const QPoint &position) {
    move(position.x(), position.y());
}
