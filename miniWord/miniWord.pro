#-------------------------------------------------
#
# Project created by QtCreator 2018-04-05T14:39:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = miniWord
TEMPLATE = app


SOURCES += main.cpp\
    view.cpp \
    controller.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

#Custom
QMAKE_CXXFLAGS += -std=c++0x

RESOURCES += \
    icons.qrc
