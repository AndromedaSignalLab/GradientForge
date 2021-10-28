#pragma once
#include <QtCore>
#include "SliderHandle.hpp"
#include <QPair>
#include <QColor>

class Sorters {
public:
    /// sort the slider list
    static bool SliderSort(const SliderHandle* a1, const SliderHandle* a2);
    /// sort the color ramp
    static bool colorRampSort(const QPair<qreal, QColor> &a1, const QPair<qreal, QColor> &a2);
};
