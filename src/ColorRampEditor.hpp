/*
ColorRampEditor class declarations
Belongs to GradientEditor Project
Copyright (C) 2021 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once

#include <QWidget>
#include <QColorDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include "ColorRamp.hpp"
#include <QUuid>
#include <QColor>

#include <iostream>
using namespace std;

class SliderHandle;
class ColorRampWidget;
class MultiHandleSlider;
class QSliderTextWidget;
class ColorRampEditor : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool Vertical READ isVertical WRITE setVertical)

public:

    /// Basic Constructor
    ColorRampEditor(QWidget* parent=0, Qt::Orientation orientation = Qt::Horizontal);

    /// Destructor
    virtual ~ColorRampEditor();

    bool isVertical() const;
    void setVertical(const bool &vertical);

    ColorRamp getColorRamp();
    void setColorRamp(const ColorRamp &colorRamp);

	/// return a 256 colortable from the ramp
	QVector<QRgb> getColorTable();

signals:

    /// signal that hide is changed
    void colorRampChanged();
    void colorClicked(double value, QColor color);
    void sliderValueChanged(QUuid sliderId, qreal value);

public slots:
    void onColorClicked(double value, QColor color);
    void onSliderValueChanged(QUuid sliderId, qreal value);
    void onColorRampChanged();

protected slots:

    /// resize required
    virtual void resizeEvent(QResizeEvent* e);

    /// detect a mouse is pressed
    //virtual void mousePressEvent(QMouseEvent* e);

protected:
    /// the orientation
    Qt::Orientation orientation;

    /// the widgets drawint the ramp, the sliders, the text
    ColorRampWidget* colorRampWidget;
    MultiHandleSlider* multiHandleSlider;
    QGridLayout* verticalLayout;
    QGridLayout* horizontalLayout;
    QSpacerItem *leftSpacer, *rightSpacer, *topSpacer, *bottomSpacer;
    void changeLayout(QLayout *newLayout);
    void updateColorRamp();
};
