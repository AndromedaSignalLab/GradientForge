#pragma once

#include <QWidget>
#include <QColorDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include "ColorRamp.hpp"
#include <QUuid>
#include <QColor>

#include <iostream>
using namespace std;

class SliderHandle;
class ColorRampWidget;
class MultiHandleSlider;
class QSliderTextWidget;
class ColorRampEditor : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool Vertical READ isVertical WRITE setVertical)

public:

    /// Basic Constructor
    ColorRampEditor(QWidget* parent=0, Qt::Orientation orientation = Qt::Horizontal);

    /// Destructor
    virtual ~ColorRampEditor();

    bool isVertical() const;
    void setVertical(const bool &vertical);

    ColorRamp getColorRamp();
    void setColorRamp(const ColorRamp &colorRamp);

	/// return a 256 colortable from the ramp
	QVector<QRgb> getColorTable();

signals:

    /// signal that hide is changed
    void colorRampChanged();
    void colorClicked(double value, QColor color);
    void sliderValueChanged(QUuid sliderId, qreal value);

public slots:
    void onColorClicked(double value, QColor color);
    void onSliderValueChanged(QUuid sliderId, qreal value);
    void onColorRampChanged();

protected slots:

    /// resize required
    virtual void resizeEvent(QResizeEvent* e);

    /// detect a mouse is pressed
    //virtual void mousePressEvent(QMouseEvent* e);

protected:
    /// the orientation
    Qt::Orientation orientation;

    /// the widgets drawint the ramp, the sliders, the text
    ColorRampWidget* colorRampWidget;
    MultiHandleSlider* multiHandleSlider;
    QGridLayout* verticalLayout;
    QGridLayout* horizontalLayout;
    QSpacerItem *leftSpacer, *rightSpacer, *topSpacer, *bottomSpacer;
    void changeLayout(QLayout *newLayout);
    void updateColorRamp();
};
