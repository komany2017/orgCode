#-------------------------------------------------
#
# Project created by QtCreator 2017-02-20T10:28:53
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Delegate
TEMPLATE = app




SOURCES += main.cpp\
        mainwindow.cpp \
    qdownload.cpp \
    dialog.cpp

HEADERS  += mainwindow.h \
    qdownload.h \
    mymodel.h \
    dialog.h

FORMS    += mainwindow.ui \
    dialog.ui

RESOURCES += \
    style.qrc
