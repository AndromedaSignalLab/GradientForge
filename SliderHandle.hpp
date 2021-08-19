#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <QPoint>

class SliderHandle : public QWidget
{
    Q_OBJECT
public:

    /// Constructor
    SliderHandle(Qt::Orientation orientation = Qt::Horizontal, QColor col = Qt::black, QWidget* parent=0);

    /// set the color of the slider
    void setColor(QColor color);

    /// get the color
    QColor getColor();

    /// the value
    qreal value;

    inline void move(int ax, int ay);
    inline void move(const QPoint &position);

protected slots:

    /// paint the widget
    virtual void paintEvent(QPaintEvent* event);

protected:

    /// the color of the slider
    QColor color;

    /// the orientation
    Qt::Orientation orientation;
};

inline void SliderHandle::move(int ax, int ay)
{
    if (orientation==Qt::Horizontal)
        QWidget::move(ax - geometry().width()/2, ay);
    else
        QWidget::move(ax, ay - geometry().height()/2);
}

inline void SliderHandle::move(const QPoint &position) {
    move(position.x(), position.y());
}
