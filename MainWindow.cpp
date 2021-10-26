#include "MainWindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->colorRampHorizontal, &ColorRampEditor::colorClicked, this, &MainWindow::onColorClicked);
    connect(ui->colorRampVertical, &ColorRampEditor::colorClicked, this, &MainWindow::onColorClicked);

    connect(ui->colorRampHorizontal, &ColorRampEditor::sliderValueChanged, this, &MainWindow::onSliderValueChanged);
    connect(ui->colorRampVertical, &ColorRampEditor::sliderValueChanged, this, &MainWindow::onSliderValueChanged);
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

