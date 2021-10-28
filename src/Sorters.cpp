#include "Sorters.hpp"

bool Sorters::colorRampSort(const QPair<qreal, QColor> &a1, const QPair<qreal, QColor> &a2)
{
    return a1.first < a2.first;
}

// -----------------------------------------------------------
bool Sorters::SliderSort(const SliderHandle* a1, const SliderHandle* a2)
{
    return a1->getValue() < a2->getValue();
}
