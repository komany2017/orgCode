#-------------------------------------------------
#
# Project created by QtCreator 2017-01-22T12:07:42
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LogsAnalyse
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES += main.cpp\
        mainwindow.cpp \

HEADERS  += mainwindow.h \

FORMS    += mainwindow.ui

MOC_DIR = tmp/moc

RCC_DIR = tmp/rc

UI_DIR = tmp/ui

OBJECTS_DIR = tmp/obj



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/QuaZIP/lib/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/QuaZIP/lib/ -lquazipd

INCLUDEPATH += $$PWD/QuaZIP/inlcude
DEPENDPATH += $$PWD/QuaZIP/inlcude

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/QuaZIP/lib/libquazip.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/QuaZIP/lib/libquazipd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/QuaZIP/lib/quazip.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/QuaZIP/lib/quazipd.lib
