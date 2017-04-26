#-------------------------------------------------
#
# Project created by QtCreator 2015-12-03T18:39:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SettingPanel
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        settingpanel.cpp \
    epushbutton.cpp

HEADERS  += settingpanel.h \
    epushbutton.h

TRANSLATIONS += settingpanel.ts

RESOURCES += \
    settingpanel.qrc
