#pragma once

#include <QMainWindow>
#include <QUuid>
#include <QColor>

namespace Ui {
class ColorRampEditorTestWindow;
}

class ColorRampEditorTestWindow : public QMainWindow
{
    Q_OBJECT

        public:
                 explicit ColorRampEditorTestWindow(QWidget *parent = nullptr);
    ~ColorRampEditorTestWindow();
public slots:
    void onColorClicked(double value, QColor color);
    void onSliderValueChanged(QUuid sliderId, qreal value);

protected:
    Ui::ColorRampEditorTestWindow *ui;
    void closeEvent(QCloseEvent *event);
};

