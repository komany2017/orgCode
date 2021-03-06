# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = lib

DESTDIR = $$PWD/../bin

CONFIG(debug, debug|release) {
    TARGET = quazipd
} else {
    TARGET = quazip
}

QT += core
CONFIG += debug_and_release
DEFINES += _WINDOWS QUAZIP_BUILD NOMINMAX QT_DLL
INCLUDEPATH += . \
               ./release \
               $(QTDIR)/mkspecs/win32-msvc2010

DEPENDPATH += .
MOC_DIR += release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

HEADERS += ./JlCompress.h \
    ./crypt.h \
    ./ioapi.h \
    ./quaadler32.h \
    ./quachecksum32.h \
    ./quacrc32.h \
    ./quazip.h \
    ./quazip_global.h \
    ./quazipdir.h \
    ./quazipfileinfo.h \
    ./quazipnewinfo.h \
    ./unzip.h \
    ./zip.h \
    ./quagzipfile.h \
    ./quaziodevice.h \
    ./quazipfile.h

SOURCES += ./JlCompress.cpp \
    ./qioapi.cpp \
    ./quaadler32.cpp \
    ./quacrc32.cpp \
    ./quagzipfile.cpp \
    ./quaziodevice.cpp \
    ./quazip.cpp \
    ./quazipdir.cpp \
    ./quazipfile.cpp \
    ./quazipfileinfo.cpp \
    ./quazipnewinfo.cpp \
    ./unzip.c \
    ./zip.c
