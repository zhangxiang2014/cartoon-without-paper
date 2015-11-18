#-------------------------------------------------
#
# Project created by QtCreator 2015-11-12T16:32:15
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = $$qtLibraryTarget(lefttoolbar)
TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../MainApp
DESTDIR = ../../MainApp/plugins

SOURCES += lefttoolbar.cpp\
    penshapeboard.cpp \
    colorbar.cpp \
    materiallib.cpp \
    template.cpp \
    libxmlstreamreader.cpp

HEADERS += lefttoolbar.h\
    penshapeboard.h \
    colorbar.h \
    materiallib.h \
    template.h \
    libxmlstreamreader.h \
    leftsignalbox.h

DISTFILES += \
    lefttoolbar.json

RESOURCES += \
    lefttoolbar_res.qrc
