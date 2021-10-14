#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <QPoint>
#include "MathUtil.hpp"

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

    /// set the color of the slider
    void setColor(QColor color);

    /// get the color
    QColor getColor() const;
    int getBoundarySpace();

    /// the value
    //qreal value;
    qreal getValue() const;
    void setValue(qreal value);
    void update();

    inline void move(int ax, int ay);
    inline void move(const QPoint &position);

protected slots:

    /// paint the widget
    virtual void paintEvent(QPaintEvent* event);

protected:
    SliderHandleProperties properties;
    QWidget *parent;
    qreal value;
    qreal calculateNominalValueFromPosition();
};

inline void SliderHandle::move(int ax, int ay)
{
    if (properties.orientation==Qt::Horizontal) {
        //value = MathUtil::getNormalizedValue(ax, )
        QWidget::move(ax - properties.width/2, ay);
    }
    else {
        QWidget::move(ax, ay - properties.height/2);
    }
}

inline void SliderHandle::move(const QPoint &position) {
    move(position.x(), position.y());
}
