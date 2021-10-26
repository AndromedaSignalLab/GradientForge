QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += link_pkgconfig

SOURCES += \
    ColorRampEditor.cpp \
    ColorRampEditorTestWindow.cpp \
    ColorRampWidget.cpp \
    ColorRampWidgetTestWindow.cpp \
    ColorUtil.cpp \
    MathUtil.cpp \
    MultiHandleSlider.cpp \
    MultiHandleSliderTestWindow.cpp \
    SliderHandle.cpp \
    SliderUtil.cpp \
    Sorters.cpp \
    main.cpp

HEADERS += \
    ColorRamp.hpp \
    ColorRampEditor.hpp \
    ColorRampEditorTestWindow.hpp \
    ColorRampWidget.hpp \
    ColorRampWidgetTestWindow.hpp \
    ColorUtil.hpp \
    MathUtil.hpp \
    MultiHandleSlider.hpp \
    MultiHandleSliderTestWindow.hpp \
    SliderHandle.hpp \
    SliderUtil.hpp \
    Sorters.hpp

FORMS += \
    ColorRampEditorTestWindow.ui \
    ColorRampWidgetTestWindow.ui \
    MultiHandleSliderTestWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
