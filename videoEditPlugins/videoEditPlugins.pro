#-------------------------------------------------
#
# Project created by QtCreator 2015-11-03T14:38:21
#
#-------------------------------------------------

QT       += widgets multimedia

TARGET = videoEditPlugins
TEMPLATE = lib

DEFINES += VIDEOEDITPLUGINS_LIBRARY

SOURCES += videoeditplugins.cpp \
    dialog.cpp

HEADERS +=\
        videoeditplugins_global.h \
    dialog.h \
    videoeditplugins.h \
    videoeditinterface.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    dialog.ui
