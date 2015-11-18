#ifndef VIDEOEDITPLUGINS_H
#define VIDEOEDITPLUGINS_H
#include "qobject.h"
#include "videoeditinterface.h"
#include "dialog.h"
#include "videoeditplugins_global.h"
#include "qdockwidget.h"
class VIDEOEDITPLUGINSSHARED_EXPORT VideoEditPlugins:public QObject,VideoEditInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID VideoEditInterface_iid) // FILE "videoeditplugins.json"
    Q_INTERFACES(VideoEditInterface)
public:
    VideoEditPlugins();
    void showVideoEditMain(void);
    QDockWidget* getTheDialog();
    QDockWidget* getDockWidget();
private:
    Dialog* w ;
};

#endif // VIDEOEDITPLUGINS_H
