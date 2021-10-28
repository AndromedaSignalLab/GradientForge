#pragma once

#include <QColor>
#include "ColorRamp.hpp"

class ColorUtil
{
public:
    ColorUtil();
    static QColor getColor(qreal key, ColorRampMap colorRamp);
    static QColor getColor(qreal key, ColorRamp colorRamp);
};

