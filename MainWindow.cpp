#include "MainWindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->colorRampHorizontal, &QColorRampEditor::colorClicked, this, &MainWindow::onColorClicked);
    connect(ui->colorRampVertical, &QColorRampEditor::colorClicked, this, &MainWindow::onColorClicked);

    connect(ui->colorRampHorizontal, &QColorRampEditor::sliderValueChanged, this, &MainWindow::onSliderValueChanged);
    connect(ui->colorRampVertical, &QColorRampEditor::sliderValueChanged, this, &MainWindow::onSliderValueChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onColorClicked(double value, QColor color)
{
    ui->colorRampValue->setText(QString::number(value));
    ui->colorRampColor->setText(color.name());
}

void MainWindow::onSliderValueChanged(QUuid sliderId, qreal value)
{
    ui->sliderId->setText(sliderId.toString());
    ui->sliderValue->setText(QString::number(value));
}

