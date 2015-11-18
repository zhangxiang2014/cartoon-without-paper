#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPluginLoader>
#include <QTimer>

#include <QScrollArea>
#include <QMessageBox>
#include <QActionGroup>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QColorDialog>
#include <QInputDialog>
#include <QApplication>
#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setObjectName("MainWindow");

    if (!loadPlugin()||!loadLibrary()) {
        QMessageBox::information(this, "Error", "Could not load the plugin");
    }
     testButton = new QPushButton;
     pencil = customWidgetInterface->createWidget(this);
     videoEditInterface->getDockWidget()->setTitleBarWidget(new QWidget);
     videoEditInterface->getDockWidget()->setFloating(false);

     //connect(testButton,SIGNAL(clicked(bool)),this,SLOT(showMain()));
     connect(videoEditInterface->getTheDialog(),SIGNAL(saveCurrentFrameSignal(int)),pencil,SLOT(saveProject()));
     connect(pencil,SIGNAL(postURL(QString,QString)),videoEditInterface->getTheDialog(),SLOT(loadCurrentFrame(QString,QString)));
     connect(videoEditInterface->getTheDialog(),SIGNAL(openThisPicture(QString,int)),pencil,SLOT(reopenProject(QString)));
     ((QMainWindow*)pencil)->addDockWidget(Qt::BottomDockWidgetArea,videoEditInterface->getDockWidget());




//     m_leftToolBar = loadUIPlugin();
//     if(m_leftToolBar != NULL){
//         m_leftToolBar->install((QMainWindow*)pencil);
//         connect(m_leftToolBar->getSignalBox(),
//                          SIGNAL(whichToolsBoardBtnWasClicked(int)),
//                          pencil, SLOT(whatToolOpen(int)));
//         connect(m_leftToolBar->getSignalBox(), SIGNAL(colorChanged(QColor)),
//                          pencil, SLOT(UIsetColor(QColor)));
//         connect(m_leftToolBar->getSignalBox(),
//                          SIGNAL(materialImagePathChanged(QString)),
//                          pencil, SLOT(importMaterial(QString)));
//     }
//     sliderWidthValue(int newWidth);
//    sliderFeatherValue(int newFeather);
     m_leftToolBar = loadUIPlugin<LeftToolBarInterface *>();
     if(m_leftToolBar != NULL){
         m_leftToolBar->install((QMainWindow*)pencil);
         QObject::connect(m_leftToolBar->getSignalBox(),
                          SIGNAL(whichToolsBoardBtnWasClicked(int)),
                          pencil, SLOT(whatToolOpen(int)));
         QObject::connect(m_leftToolBar->getSignalBox(),
                          SIGNAL(penWidthChanged(int)),
                          pencil, SLOT(sliderWidthValue(int)));
         QObject::connect(m_leftToolBar->getSignalBox(),
                          SIGNAL(penFeatherChanged(int)),
                          pencil, SLOT(sliderFeatherValue(int)));
         QObject::connect(m_leftToolBar->getSignalBox(), SIGNAL(colorChanged(QColor)),
                          pencil, SLOT(UIsetColor(QColor)));
         QObject::connect(m_leftToolBar->getSignalBox(),
                          SIGNAL(materialImagePathChanged(QString)),
                          pencil, SLOT(importMaterial(QString)));
     }

     m_topToolBar = loadUIPlugin<TopToolBarInterface *>();
     if(m_topToolBar != NULL){
         m_topToolBar->install((QMainWindow*)pencil);
         QObject::connect(m_topToolBar->getSignalBox(),
                          SIGNAL(whichTopToolBarBtnWasClicked(int)),
                          pencil, SLOT(topToolOpen(int)));
     }
     resize(250, 600);



}

void MainWindow::test(QString Project_URL,QString Image_URL)
{
    QMessageBox::information(this,Project_URL,Image_URL);
}

bool MainWindow::loadPlugin()
{
    QDir pluginsDir("./");
//    QDir pluginsDir(qApp->applicationDirPath());
//#if defined(Q_OS_WIN)
//    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
//        pluginsDir.cdUp();
//#elif defined(Q_OS_MAC)
//    if (pluginsDir.dirName() == "MacOS") {
//        pluginsDir.cdUp();
//        pluginsDir.cdUp();
//        pluginsDir.cdUp();
//    }
//#endif
//    //pluginsDir.cd("../build-pencil-Desktop_Qt_5_5_1_MSVC2013_32bit-Debug/app/debug");
//    pluginsDir.cd("./");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            loadDrawInterface = qobject_cast<LoadDrawInterface *>(plugin);
            customWidgetInterface =qobject_cast<QDesignerCustomWidgetInterface *>(plugin);
        //    customWidgetCollection =qobject_cast<QDesignerCustomWidgetCollectionInterface *>(plugin);
            if (customWidgetInterface)
                return true;
        }
    }

    return false;
}

bool MainWindow::loadLibrary()
{
    //load the library
    QDir pluginDir("./");
    //遍历插件目录
    foreach(QString fileName,pluginDir.entryList(QDir::Files))
    {
        QPluginLoader pluginLoader(pluginDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if(plugin)
        {
            videoEditInterface = qobject_cast<VideoEditInterface*>(plugin);
            if(videoEditInterface)
            {
                return true;
            }
        }
    }
    return false;
}

void MainWindow::showMain()
{
    //show the dialog
    QMessageBox::information(this, "dialog", " the plugin");
    videoEditInterface->showVideoEditMain();
}

MainWindow::~MainWindow()
{

}
