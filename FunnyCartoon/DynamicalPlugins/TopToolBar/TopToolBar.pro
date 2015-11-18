#-------------------------------------------------
#
# Project created by QtCreator 2015-11-15T10:24:52
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = $$qtLibraryTarget(toptoolbar)
TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ../../MainApp
DESTDIR = ../../MainApp/plugins

SOURCES += toptoolbar.cpp

HEADERS += toptoolbar.h \
    topsignalbox.h

DISTFILES += \
    toptoolbar.json

RESOURCES += \
    toptoolbar_res.qrc
