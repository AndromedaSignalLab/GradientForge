#ifndef COLORRAMPEDITORTESTWINDOW_H
#define COLORRAMPEDITORTESTWINDOW_H

#include <QMainWindow>
#include <QUuid>
#include <QColor>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ColorRampEditorTestWindow : public QMainWindow
{
    Q_OBJECT

public:
    ColorRampEditorTestWindow(QWidget *parent = nullptr);
    ~ColorRampEditorTestWindow();
public slots:
    void onColorClicked(double value, QColor color);
    void onSliderValueChanged(QUuid sliderId, qreal value);

protected:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent *event);
};
#endif // COLORRAMPEDITORTESTWINDOW_H
