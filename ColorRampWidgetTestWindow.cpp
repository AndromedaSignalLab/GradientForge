#include "ColorRampWidgetTestWindow.hpp"
#include "ui_ColorRampWidgetTestWindow.h"

ColorRampWidgetTestWindow::ColorRampWidgetTestWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ColorRampWidgetTestWindow)
{
    ui->setupUi(this);
}

ColorRampWidgetTestWindow::~ColorRampWidgetTestWindow()
{
    delete ui;
}
