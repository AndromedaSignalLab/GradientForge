#include "ColorRampEditorTestWindow.hpp"
#include "MultiHandleSliderTestWindow.hpp"
#include "ColorRampWidgetTestWindow.hpp"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ColorRampEditorTestWindow colorRampEditorTestWindow;
    colorRampEditorTestWindow.show();

    MultiHandleSliderTestWindow multiHandleSliderTestWindow;
    multiHandleSliderTestWindow.show();

    ColorRampWidgetTestWindow colorRampWidgetTestWindow;
    colorRampWidgetTestWindow.show();

    return a.exec();
}
