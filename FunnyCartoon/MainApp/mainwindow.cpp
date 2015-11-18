#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setObjectName("MainWindow");
    m_leftToolBar = loadPlugin<LeftToolBarInterface *>();
    if(m_leftToolBar != NULL){
        m_leftToolBar->install(this);
        QObject::connect(m_leftToolBar->getSignalBox(),
                         SIGNAL(whichToolsBoardBtnWasClicked(int)),
                         this, SLOT(displayToolsBoardBtnID(int)));
        QObject::connect(m_leftToolBar->getSignalBox(),
                         SIGNAL(penWidthChanged(int)),
                         this, SLOT(displayPenWidth(int)));
        QObject::connect(m_leftToolBar->getSignalBox(),
                         SIGNAL(penFeatherChanged(int)),
                         this, SLOT(displayPenFeather(int)));
        QObject::connect(m_leftToolBar->getSignalBox(), SIGNAL(colorChanged(QColor)),
                     this, SLOT(displayColor(QColor)));
        QObject::connect(m_leftToolBar->getSignalBox(),
                         SIGNAL(materialImagePathChanged(QString)),
                         this, SLOT(displayImagePath(QString)));
    }

    m_topToolBar = loadPlugin<TopToolBarInterface *>();
    if(m_topToolBar != NULL){
        m_topToolBar->install(this);
        QObject::connect(m_topToolBar->getSignalBox(),
                         SIGNAL(whichTopToolBarBtnWasClicked(int)),
                         this, SLOT(displayTopToolBarBtnID(int)));
    }

    resize(850, 650);
}

MainWindow::~MainWindow()
{
}

/* the slot functions */
void MainWindow::displayToolsBoardBtnID(int btnID)
{
    qDebug() << "tools board btn" << btnID << " was checked.";
}

void MainWindow::displayPenWidth(int newWidth)
{
    qDebug() << "the new pen width: " << newWidth;
}

void MainWindow::displayPenFeather(int newFeather)
{
    qDebug() << "the new pen feather: " << newFeather;
}

void MainWindow::displayColor(QColor newColor)
{
    qDebug() << "Red:" << newColor.red()\
             << "Green:" << newColor.green()\
             << "Blue:" << newColor.blue();
}

void MainWindow::displayImagePath(QString newImagePath)
{
    qDebug() << "new material library image path:" << newImagePath;
}

void MainWindow::displayTopToolBarBtnID(int btnID)
{
    qDebug() << "top tool bar btn" << btnID << " was clicked.";
}
