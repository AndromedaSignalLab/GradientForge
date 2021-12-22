/*
ColorRampWidget class declarations
Belongs to GradientEditor Project
Copyright (C) 2021 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once

#include <QWidget>
#include "ColorRamp.hpp"

class ColorRampWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool Vertical READ isVertical WRITE setVertical)
public:
    ColorRampWidget(QWidget* parent=NULL, Qt::Orientation orientation = Qt::Horizontal);

    QColor getColor(qreal key) const;
    const ColorRamp & getcolorRamp() const;
    void setColorRamp(const ColorRamp & newColorRamp);

    bool isVertical() const;
    void setVertical(const bool &vertical);

signals:
    void colorClicked(double value, QColor color);

public slots:
    void onColorRampChanged();

protected slots:
    virtual void mousePressEvent(QMouseEvent* e);

protected:
    ColorRamp colorRamp;
    void paintEvent(QPaintEvent* e);
    Qt::Orientation orientation;
};
