/*
ColorRampEditorTestWindow class definitions
Belongs to GradientEditor Project
Copyright (C) 2021 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "ColorRampEditorTestWindow.hpp"
#include "ui_ColorRampEditorTestWindow.h"

ColorRampEditorTestWindow::ColorRampEditorTestWindow(QWidget *parent) :
    QMainWindow(parent),
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

void ColorRampEditorTestWindow::closeEvent(QCloseEvent *)
{
    QApplication::exit();
}
