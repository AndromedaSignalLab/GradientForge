#pragma once

#include <QWidget>
#include "ColorRamp.hpp"

class ColorRampWidget : public QWidget
{
    Q_OBJECT
public:
    ColorRampWidget(QWidget* parent=NULL, Qt::Orientation orientation = Qt::Horizontal);

    QColor getColor(qreal key) const;
    const ColorRamp & getcolorRamp() const;
    void setColorRamp(const ColorRamp & newColorRamp);

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
