/****************************************************************************
**
** Copyright (c) 2012 Richard Steffen and/or its subsidiary(-ies).
** All rights reserved.
** Contact: rsteffen@messbild.de, rsteffen@uni-bonn.de
**
** QColorRampEditor is free to use unter the terms of the LGPL 2.1 License in
** Free and Commercial Products.
****************************************************************************/

#pragma once

#include <QWidget>
#include <QColorDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>

#include <iostream>
using namespace std;

// -----------------------------------------------------------
// QColorRampEditor ------------------------------------------
// -----------------------------------------------------------
class QColorRampEditorSlider;
class QRampWidget;
class QSlidersWidget;
class QSliderTextWidget;
class QColorRampEditor : public QWidget
{
    Q_OBJECT
public:

    /// Basic Constructor
    QColorRampEditor(QWidget* parent=0, int orientation = Qt::Vertical);

    /// Destructor
    virtual ~QColorRampEditor();

    // define friends to access protected members
    friend class QRampWidget;
    friend class QSlidersWidget;
    friend class QSliderTextWidget;


	/// return a 256 colortable from the ramp
	QVector<QRgb> getColorTable();

signals:

    /// signal that hide is changed
    void rampChanged();

public slots:


protected slots:

    /// resize required
    virtual void resizeEvent(QResizeEvent* e);

    /// detect a mouse is pressed
    virtual void mousePressEvent(QMouseEvent* e);    

    /// on update the ramp because sliders are changed
    virtual void updateRamp();

protected:
    /// the orientation
    int orientation;

    /// the widgets drawint the ramp, the sliders, the text
    QRampWidget* rampwid_;
    QSlidersWidget* slidewid_;
};

class QRampWidget : public QWidget
{
public:
    QRampWidget(QWidget* parent=NULL);
    QColorRampEditor* rampeditor_;

protected:
    void paintEvent(QPaintEvent* e);
};
