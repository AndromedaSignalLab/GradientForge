#pragma once

#include <QWidget>
#include "ColorRamp.hpp"

class ColorRampWidget : public QWidget
{
    Q_OBJECT
public:
    ColorRampWidget(Qt::Orientation orientation = Qt::Horizontal, QWidget* parent=NULL, int boundarySpace = 12);

    QColor getColor(qreal key) const;
signals:
    void colorClicked(double value, QColor color);

public slots:
    void onColorRampChanged(ColorRamp colorRamp);

protected slots:
    virtual void mousePressEvent(QMouseEvent* e);

protected:
    ColorRamp colorRamp;
    void paintEvent(QPaintEvent* e);
    Qt::Orientation orientation;
    int boundarySpace;
};
