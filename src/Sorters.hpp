/*
Sorters class declarations
Belongs to GradientEditor Project
Copyright (C) 2021 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once
#include <QtCore>
#include "SliderHandle.hpp"
#include <QPair>
#include <QColor>
#include <QGradientStop>

class Sorters {
public:
    /// sort the color ramp
    static bool colorRampSort(const QGradientStop &a1, const QGradientStop &a2);

    /// sort the slider list
    static bool SliderSort(const SliderHandle *a1, const SliderHandle* a2);
};
