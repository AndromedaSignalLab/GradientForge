#include "MultiHandleSliderTestWindow.hpp"
#include "ui_MultiHandleSliderTestWindow.h"

MultiHandleSliderTestWindow::MultiHandleSliderTestWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MultiHandleSliderTestWindow)
{
    ui->setupUi(this);
    SliderHandle* sl = ui->multiHandleSlider->addSlider(0.5, Qt::red);
    if(sl != nullptr)
        QObject::connect(ui->multiHandleSlider, &MultiHandleSlider::sliderValueChanged, this, &MultiHandleSliderTestWindow::onSliderValueChanged);

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
        ui->multiHandleSlider->addSlider(value, Qt::blue);
    }
}

