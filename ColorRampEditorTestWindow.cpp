#include "ColorRampEditorTestWindow.hpp"
#include "ui_ColorRampEditorTestWindow.h"

ColorRampEditorTestWindow::ColorRampEditorTestWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::ColorRampEditorTestWindow)
{
    ui->setupUi(this);
    connect(ui->colorRampHorizontal, &ColorRampEditor::colorClicked, this, &ColorRampEditorTestWindow::onColorClicked);
    connect(ui->colorRampVertical, &ColorRampEditor::colorClicked, this, &ColorRampEditorTestWindow::onColorClicked);

    connect(ui->colorRampHorizontal, &ColorRampEditor::sliderValueChanged, this, &ColorRampEditorTestWindow::onSliderValueChanged);
    connect(ui->colorRampVertical, &ColorRampEditor::sliderValueChanged, this, &ColorRampEditorTestWindow::onSliderValueChanged);
}

ColorRampEditorTestWindow::~ColorRampEditorTestWindow()
{
    delete ui;
}

void ColorRampEditorTestWindow::onColorClicked(double value, QColor color)
{
    ui->colorRampValue->setText(QString::number(value));
    ui->colorRampColor->setText(color.name());
}

void ColorRampEditorTestWindow::onSliderValueChanged(QUuid sliderId, qreal value)
{
    ui->sliderId->setText(sliderId.toString());
    ui->sliderValue->setText(QString::number(value));
}

void ColorRampEditorTestWindow::closeEvent(QCloseEvent * event)
{
    QApplication::exit();
}

