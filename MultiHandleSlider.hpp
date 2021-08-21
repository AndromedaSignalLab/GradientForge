#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QColorDialog>
#include "SliderHandle.hpp"
typedef QVector<QPair<qreal, QColor>> ColorRamp;

class MultiHandleSlider : public QWidget
{
    Q_OBJECT
public:
    /// Constructor
    MultiHandleSlider(Qt::Orientation orientation = Qt::Horizontal, QWidget* parent=NULL);
    ~MultiHandleSlider();

    /// get the number of sliders
    int getSliderNum();

    /// return the Ramp definition
    ColorRamp getRamp();

    /// set Ramp definition
    void setRamp(ColorRamp ramp);

    /// get the value of a slider
    qreal updateValue(SliderHandle* sl);

    /// get the position
    int updatePos(SliderHandle* sl);

    qreal getNormalizedValue(qreal value);

    /// set a color choose dlg
    void setColorChoose(QColorDialog* coldlg);
    int getBoundarySpace();
    void addSlider(const QPoint &position, const QColor &color, bool skipIfExists = true);
    void addSlider(const double &value, const QColor &color);

signals:
    void colorRampChanged(ColorRamp colorRamp);

public slots:
    /// set the color of a slider to zero
    void setSliderColor(int index, QColor col);

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
    int activeSlider;
    /// a color chooser dlg
    QColorDialog* colorChooseDialog;
    /// all poses with its sliders
    QList<SliderHandle*> sliderHandles;

    QRect getContentsRectangle();
    qreal getValueFromPosition(const QPoint &position);
    QPoint getPositionForValue(qreal value, qreal sliderWidth, qreal sliderHeight);

    /// the orientation
    Qt::Orientation orientation;
    SliderHandleProperties handleProperties;
    void removeActiveSlider();
};
