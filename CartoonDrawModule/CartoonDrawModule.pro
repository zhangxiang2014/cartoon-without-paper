#-------------------------------------------------
#
# Project created by QtCreator 2015-11-04T14:06:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CartoonDrawModule
TEMPLATE = app

RESOURCES += \
    cartoonres.qrc

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    drawmoduleinterface.h \
    videoeditinterface.h \
    interfaces.h \
    leftsignalbox.h \
    topsignalbox.h

FORMS    += mainwindow.ui


#LIBS           = -L../build-pencil-Desktop_Qt_5_5_1_MSVC2013_32bit-Debug/app/debug -lapp_draw
#if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
#   mac:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)_debug
#   win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
#}




