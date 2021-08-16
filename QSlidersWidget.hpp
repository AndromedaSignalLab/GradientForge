#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QColorDialog>
#include "QColorRampEditor.hpp"

class QSlidersWidget : public QWidget
{
    Q_OBJECT
public:
    /// Constructor
    QSlidersWidget(int orientation = Qt::Horizontal, QWidget* parent=NULL);
    ~QSlidersWidget();

    /// get the number of sliders
    int getSliderNum();

    /// return the Ramp definition
    QVector<QPair<qreal, QColor> > getRamp();

    /// set Ramp definition
    void setRamp(QVector<QPair<qreal, QColor> > ramp);

    /// get the value of a slider
    qreal updateValue(QColorRampEditorSlider* sl);

    /// get the position
    int updatePos(QColorRampEditorSlider* sl);

    qreal getNormalizedValue(qreal value);


    QColorRampEditor* rampEditor;

    /// set a color choose dlg
    void setColorChoose(QColorDialog* coldlg);
    int getBoundSpace();
    void addSlider(int position, QColor color);

signals:
    void sliderMoved(int sliderId, qreal value);

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
    QList<QColorRampEditorSlider*> sliders;


    /// the orientation
    int orientation;

    /// bound space
    int bspace_;

    /// min and max value from initialization
    qreal mi_, ma_;
};



// -----------------------------------------------------------
// QColorRampEditorSlider ------------------------------------
// -----------------------------------------------------------
class QColorRampEditorSlider : public QWidget
{
    Q_OBJECT
public:

    /// Constructor
    QColorRampEditorSlider(int orientation = Qt::Horizontal, QColor col = Qt::black, QWidget* parent=0);

    /// set the color of the slider
    void setColor(QColor color);

    /// get the color
    QColor getColor();

    /// the value
    qreal value;

protected slots:

    /// paint the widget
    virtual void paintEvent(QPaintEvent* event);

protected:

    /// the color of the slider
    QColor color;

    /// the orientation
    int orientation;
};
