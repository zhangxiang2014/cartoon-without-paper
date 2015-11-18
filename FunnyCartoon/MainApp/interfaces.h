#ifndef INTERFACES_H
#define INTERFACES_H

#include <QMainWindow>
#include "leftsignalbox.h"
#include "topsignalbox.h"

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

#define LeftToolBarInterface_iid "com.linwm.FunnyCartoon.MainApp.LeftToolBarInterface"
Q_DECLARE_INTERFACE(LeftToolBarInterface, LeftToolBarInterface_iid)

#define TopToolBarInterface_iid "com.linwm.FunnyCartoon.MainApp.TopToolBarInterface"
Q_DECLARE_INTERFACE(TopToolBarInterface, TopToolBarInterface_iid)

#endif // INTERFACES_H

