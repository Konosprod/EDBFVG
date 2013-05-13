#-------------------------------------------------
#
# Project created by QtCreator 2013-04-13T12:07:04
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EDBFVG
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    editdialog.cpp

HEADERS  += mainwindow.h \
    editdialog.h

FORMS    += mainwindow.ui \
    editdialog.ui

RESOURCES += \
    rc.qrc
