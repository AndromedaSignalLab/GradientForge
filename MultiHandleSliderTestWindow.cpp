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

