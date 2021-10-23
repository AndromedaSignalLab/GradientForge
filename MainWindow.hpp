#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUuid>
#include <QColor>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void onColorClicked(double value, QColor color);
    void onSliderValueChanged(QUuid sliderId, qreal value);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
