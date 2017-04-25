#-------------------------------------------------
#
# Project created by QtCreator 2017-03-01T13:25:01
#
#-------------------------------------------------
include (C:\qtwinmigrate-2.8_1-opensource_qt5\common.pri)
include (C:\qtwinmigrate-2.8_1-opensource_qt5\src\qtwinmigrate.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Login
TEMPLATE = lib

DEFINES += LOGIN_LIBRARY

SOURCES += login.cpp \
    dialog.cpp \
    dialogwait.cpp

HEADERS += login.h\
        login_global.h \
    dialog.h \
    dialogwait.h

FORMS += \
    dialog.ui \
    dialogwait.ui


INCLUDEPATH += $$PWD/../../../../qtwinmigrate-2.8_1-opensource_qt5/src
DEPENDPATH += $$PWD/../../../../qtwinmigrate-2.8_1-opensource_qt5/src

RESOURCES +=
