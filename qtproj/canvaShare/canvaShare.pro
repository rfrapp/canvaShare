#-------------------------------------------------
#
# Project created by QtCreator 2014-09-14T18:15:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = canvaShare
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

LIBS += -lSDL2 -lSDL2_image -lSDL2_ttf

FORMS    += mainwindow.ui
