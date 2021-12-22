/*
MultiHandleSliderTestWindow class definitions
Belongs to GradientEditor Project
Copyright (C) 2021 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "MultiHandleSliderTestWindow.hpp"
#include "ui_MultiHandleSliderTestWindow.h"

MultiHandleSliderTestWindow::MultiHandleSliderTestWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MultiHandleSliderTestWindow)
{
    ui->setupUi(this);
    SliderHandle* slh = ui->multiHandleSliderHorizontal->addSlider(0.5, Qt::red);
    SliderHandle* slv = ui->multiHandleSliderVertical->addSlider(0.5, Qt::red);
    if(slh != nullptr)
        QObject::connect(ui->multiHandleSliderHorizontal, &MultiHandleSlider::sliderValueChanged, this, &MultiHandleSliderTestWindow::onSliderValueChanged);
    if(slv != nullptr)
        QObject::connect(ui->multiHandleSliderVertical, &MultiHandleSlider::sliderValueChanged, this, &MultiHandleSliderTestWindow::onSliderValueChanged);

}

MultiHandleSliderTestWindow::~MultiHandleSliderTestWindow()
{
    delete ui;
}

void MultiHandleSliderTestWindow::onSliderValueChanged(QUuid sliderId, qreal value)
{
    ui->valueLabel->setText(QString::number(value));
    ui->idLabel->setText(sliderId.toString());
}

void MultiHandleSliderTestWindow::on_pushButtonAdd_clicked()
{
    bool ok = false;
    double value = ui->lineEditValue->text().toDouble(&ok);
    if(ok && value <= 1 && value >= 0) {
        ui->multiHandleSliderHorizontal->addSlider(value, Qt::blue);
        ui->multiHandleSliderVertical->addSlider(value, Qt::blue);
    }
}

void MultiHandleSliderTestWindow::closeEvent(QCloseEvent * event)
{
    QApplication::exit();
}
