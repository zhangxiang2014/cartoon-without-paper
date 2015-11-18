#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QColor>
#include <QApplication>
#include <QDir>
#include <QPluginLoader>
#include "interfaces.h"
//#include "drawmoduleinterface.h"
//#include "videoeditinterface.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QPushButton* testButton;
    QWidget* pencil;

private:

    LoadDrawInterface *loadDrawInterface;
    QDesignerCustomWidgetInterface *customWidgetInterface;
    VideoEditInterface* videoEditInterface;
    bool loadPlugin();
    bool loadLibrary();
    LeftToolBarInterface *m_leftToolBar;
    TopToolBarInterface *m_topToolBar;

    template<class PluginInterfacePointer>
    PluginInterfacePointer loadUIPlugin()
    {
        QDir pluginsDir(qApp->applicationDirPath());
    #if defined(Q_OS_WIN)
        if(pluginsDir.dirName().toLower() == "debug"
                || pluginsDir.dirName().toLower() == "release")
            pluginsDir.cdUp();
    #elif defined(Q_OS_MAC)
        if(pluginsDir.dirName() == "MacOS") {
            pluginsDir.cdUp();
            pluginsDir.cdUp();
            pluginsDir.cdUp();
        }
    #endif
        pluginsDir.cd("plugins");
        foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
            QString pluginPath = pluginsDir.absoluteFilePath(fileName);
            QPluginLoader pluginLoader(pluginPath);
            QObject *plugin = pluginLoader.instance();
            if(plugin != NULL) {
                PluginInterfacePointer temp = qobject_cast<PluginInterfacePointer>(plugin);
                if(temp != NULL){//just for one plugin
                    return temp;
                }
            }
        }

        return NULL;
    }

private slots:
    void test(QString Project_URL,QString Image_URL);
    void showMain();



private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
