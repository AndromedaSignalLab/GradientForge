#include "ColorRampWidgetTestWindow.hpp"
#include "ui_ColorRampWidgetTestWindow.h"

ColorRampWidgetTestWindow::ColorRampWidgetTestWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ColorRampWidgetTestWindow)
{
    ui->setupUi(this);
    ColorRamp colorRamp;
    colorRamp.push_back(ColorRampElement(0, Qt::red));
    colorRamp.push_back(ColorRampElement(0.5, Qt::green));
    colorRamp.push_back(ColorRampElement(1, Qt::blue));
    ui->colorRampWidgetHorizontal->setColorRamp(colorRamp);
    ui->colorRampWidgetVertical->setColorRamp(colorRamp);
    connect(ui->colorRampWidgetHorizontal, &ColorRampWidget::colorClicked, this, &ColorRampWidgetTestWindow::onColorClicked);
    connect(ui->colorRampWidgetVertical, &ColorRampWidget::colorClicked, this, &ColorRampWidgetTestWindow::onColorClicked);
}

ColorRampWidgetTestWindow::~ColorRampWidgetTestWindow()
{
    delete ui;
}

void ColorRampWidgetTestWindow::onColorClicked(double value, QColor color)
{
    ui->valueLabel->setText(QString::number(value));
    //ui->colorLabel->setStyleSheet("background-color: '" + color.name() + "'");
    ui->colorLabel->setText(color.name());
    //static QPoint getPositionForNormalizedValue(qreal value, qreal boundarySpace, qreal sliderHandleWidth, qreal sliderWidth, Qt::Orientation orientation);
}
