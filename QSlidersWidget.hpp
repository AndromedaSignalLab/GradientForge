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
    QSlidersWidget(QWidget* parent=NULL);
    ~QSlidersWidget();

    QColorRampEditor* rampEditor;

    /// sort the slider list
    static bool SliderSort(const QColorRampEditorSlider* a1, const QColorRampEditorSlider* a2);
    /// set a color choose dlg
    void setColorChoose(QColorDialog* coldlg);

signals:
    void sliderMoved(QMouseEvent* e);

protected slots:

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
};
