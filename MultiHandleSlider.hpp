#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QColorDialog>
#include <QHash>
#include "SliderHandle.hpp"
#include <QStack>

typedef QVector<QPair<qreal, QColor>> ColorRamp;

class MultiHandleSlider : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool Vertical READ isVertical WRITE setVertical)
public:
    /// Constructor
    MultiHandleSlider(QWidget* parent=nullptr, Qt::Orientation orientation = Qt::Horizontal);
    ~MultiHandleSlider();

    /// get the number of sliders
    int getSliderAmount();

    /// return the Ramp definition
    ColorRamp getColorRamp();

    /// set Ramp definition
    void setColorRamp(ColorRamp ramp);

    qreal getNormalizedValue(qreal value);

    int getBoundarySpace();
    SliderHandle * addSlider(const QPoint &position, const QColor &color, bool skipIfExists = true);
    SliderHandle * addSlider(const double &value, const QColor &color);

    bool isVertical() const;
    void setVertical(const bool &vertical);

    void setValue(const QUuid &sliderHandleId, const qreal &value);
    qreal calculateValue(const QUuid &sliderHandleId);
    qreal getValue(const QUuid &sliderHandleId);
    void updatePosition(const QUuid &sliderHandleId);

signals:
    void sliderChanged();
    void sliderValueChanged(QUuid sliderId, qreal value);
    void sliderColorChanged(QUuid sliderId, QColor color);
    void sliderRemoved(QUuid sliderId);
    void sliderAdded(QUuid sliderId, QColor color, qreal value);

public slots:
    /// set the color of a slider to zero
    void setSliderColor(QUuid sliderId, QColor color);

protected slots:
    /// resize required
    virtual void resizeEvent(QResizeEvent* e);

    /// detect a mouse is pressed
    virtual void mousePressEvent(QMouseEvent* e);

    /// detect a mouse is moved
    virtual void mouseMoveEvent(QMouseEvent* e);

    /// detect a mouse is released
    virtual void mouseReleaseEvent(QMouseEvent* e);

    /// detect a mouse is released
    virtual void mouseDoubleClickEvent(QMouseEvent* e);

protected:
    /// the active slider
    QUuid activeSliderId;
    /// all poses with its sliders
    QHash<QUuid, SliderHandle*> sliderHandles;
    QStack<SliderHandle*> sliderHandleStack;

    qreal getValueFromPosition(const QPoint &position);
    QPoint getPositionForValue(qreal value);

    /// the orientation
    Qt::Orientation orientation;
    SliderHandleProperties handleProperties;
    void removeActiveSlider();
    void addSliderHandle(SliderHandle * sliderHandle);
    void removeSliderHandle(SliderHandle * sliderHandle);
    void raiseSliderHandle(SliderHandle * sliderHandle);
};
