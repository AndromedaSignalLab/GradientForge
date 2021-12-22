#Build script of GradientEditor
#Inspired by the design and algorithms of Richard Steffen's QColorRampEditor (https://store.kde.org/p/1132126   Contact: rsteffen@messbild.de, rsteffen@uni-bonn.de)
#Copyright (C) 2021 Volkan Orhan (Contact: volkanorhan@modlpugplayer.org)

#This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

#This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

#You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += link_pkgconfig

INCLUDEPATH += "./src"

INCLUDEPATH += "./test"

SOURCES = $$files(*.cpp, true)

HEADERS = $$files(*.hpp, true)

FORMS += $$files(*.ui, true)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
