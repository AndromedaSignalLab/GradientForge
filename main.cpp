#include "MainWindow.hpp"
#include "MultiHandleSliderTestWindow.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    MultiHandleSliderTestWindow w;
    w.show();
    return a.exec();
}
