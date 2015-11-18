#ifndef INTERFACES_H
#define INTERFACES_H

#include <QMainWindow>
#include "leftsignalbox.h"
#include "topsignalbox.h"

#include <QtPlugin>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>
#include <QtUiPlugin/QDesignerCustomWidgetCollectionInterface>

#include <QDialog>
#include <QDockWidget>

class LeftToolBarInterface
{
public:
    virtual ~LeftToolBarInterface() {}

    virtual LeftSignalBox *getSignalBox() =0;
    virtual void install(QMainWindow *mainWindow) =0;
};

class TopToolBarInterface
{
public:
    virtual ~TopToolBarInterface() {}

    virtual TopSignalBox *getSignalBox() =0;
    virtual void install(QMainWindow *mainWindow) =0;
};


class LoadDrawInterface
{
public:
    virtual ~LoadDrawInterface() {}
    virtual void LoadUI() = 0;
    virtual void setBottomUI(QWidget* videoUI) = 0;
};

class VideoEditInterface
{
public:
    virtual ~VideoEditInterface(){}
    virtual void showVideoEditMain(void)=0;
    virtual QDockWidget* getTheDialog()=0;
    virtual QDockWidget* getDockWidget()=0;

};

#define LeftToolBarInterface_iid "com.linwm.FunnyCartoon.MainApp.LeftToolBarInterface"
Q_DECLARE_INTERFACE(LeftToolBarInterface, LeftToolBarInterface_iid)

#define TopToolBarInterface_iid "com.linwm.FunnyCartoon.MainApp.TopToolBarInterface"
Q_DECLARE_INTERFACE(TopToolBarInterface, TopToolBarInterface_iid)

#define LoadDrawInterface_iid "org.DrawModuleInterface.LoadDrawInterface"
Q_DECLARE_INTERFACE(LoadDrawInterface, LoadDrawInterface_iid)

#define VideoEditInterface_iid "anobodykey.VideoEditInterface/2.0"
Q_DECLARE_INTERFACE(VideoEditInterface, VideoEditInterface_iid)

#endif // INTERFACES_H

