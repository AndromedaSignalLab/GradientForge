/*
ColorRampEditorTestWindow class declarations
Belongs to GradientEditor Project
Copyright (C) 2021 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

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
