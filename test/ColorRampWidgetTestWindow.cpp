/*
ColorRampWidgetTestWindow class definitions
Belongs to GradientEditor Project
Copyright (C) 2021 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

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

void ColorRampWidgetTestWindow::closeEvent(QCloseEvent * event)
{
    QApplication::exit();
}
