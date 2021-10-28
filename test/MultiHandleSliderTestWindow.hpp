#pragma once

#include <QMainWindow>
#include <QUuid>

namespace Ui {
class MultiHandleSliderTestWindow;
}

class MultiHandleSliderTestWindow : public QMainWindow
{
    Q_OBJECT

        public:
            explicit MultiHandleSliderTestWindow(QWidget *parent = nullptr);
            ~MultiHandleSliderTestWindow();
        public slots:
            void onSliderValueChanged(QUuid sliderId, qreal value);

        private slots:
            void on_pushButtonAdd_clicked();

        protected:
            Ui::MultiHandleSliderTestWindow *ui;
            void closeEvent(QCloseEvent *event);
};

