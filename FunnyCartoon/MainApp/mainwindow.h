#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>
#include <QApplication>
#include <QDir>
#include <QPluginLoader>
#include "interfaces.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void displayToolsBoardBtnID(int btnID);
    void displayPenWidth(int newWidth);
    void displayPenFeather(int newFeather);
    void displayColor(QColor newColor);
    void displayImagePath(QString newImagePath);
    void displayTopToolBarBtnID(int btnID);

private:
    LeftToolBarInterface *m_leftToolBar;
    TopToolBarInterface *m_topToolBar;

    template<class PluginInterfacePointer>
    PluginInterfacePointer loadPlugin()
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

};

#endif // MAINWINDOW_H
