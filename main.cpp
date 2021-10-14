#include "MainWindow.hpp"
#include "MultiHandleSliderTestWindow.hpp"
#include "ColorRampWidgetTestWindow.hpp"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow m;
    //m.show();

    //MultiHandleSliderTestWindow w;
    //w.show();

    ColorRampWidgetTestWindow c;
    c.show();

    return a.exec();
}
