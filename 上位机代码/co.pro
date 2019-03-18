#-------------------------------------------------
#
# Project created by QtCreator 2018-05-04T16:21:23
#
#-------------------------------------------------

QT      += core gui
QT      += serialport
QT      += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = co
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    displayinfo.cpp \
    Mysql_air.cpp

HEADERS  += mainwindow.h \
    displayinfo.h \
    Mysql_air.h

FORMS    += mainwindow.ui
