#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QColorDialog>
typedef QVector<QPair<qreal, QColor>> ColorRamp;

class QSlidersHandleWidget;

class QSlidersWidget : public QWidget
{
    Q_OBJECT
public:
    /// Constructor
    QSlidersWidget(Qt::Orientation orientation = Qt::Horizontal, QWidget* parent=NULL);
    ~QSlidersWidget();

    /// get the number of sliders
    int getSliderNum();

    /// return the Ramp definition
    ColorRamp getRamp();

    /// set Ramp definition
    void setRamp(ColorRamp ramp);

    /// get the value of a slider
    qreal updateValue(QSlidersHandleWidget* sl);

    /// get the position
    int updatePos(QSlidersHandleWidget* sl);

    qreal getNormalizedValue(qreal value);

    /// set a color choose dlg
    void setColorChoose(QColorDialog* coldlg);
    int getBoundSpace();
    void addSlider(const QPoint &position, const QColor &color, bool skipIfExists = true);

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
    QList<QSlidersHandleWidget*> sliders;


    /// the orientation
    Qt::Orientation orientation;

    /// bound space
    int boundarySpace;

    /// min and max value from initialization
    qreal mi_, ma_;
    void removeActiveSlider();
};

class QSlidersHandleWidget : public QWidget
{
    Q_OBJECT
public:

    /// Constructor
    QSlidersHandleWidget(Qt::Orientation orientation = Qt::Horizontal, QColor col = Qt::black, QWidget* parent=0);

    /// set the color of the slider
    void setColor(QColor color);

    /// get the color
    QColor getColor();

    /// the value
    qreal value;

    inline void move(int ax, int ay);

protected slots:

    /// paint the widget
    virtual void paintEvent(QPaintEvent* event);

protected:

    /// the color of the slider
    QColor color;

    /// the orientation
    Qt::Orientation orientation;
};
