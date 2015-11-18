#include "toptoolbar.h"
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QString>
#include <QToolButton>
#include <QIcon>
#include <QDebug>

TopToolBar::TopToolBar(QWidget *parent) :
    QToolBar(parent)
{
    m_signalBox = new TopSignalBox(this);
    addToolButtons();
}

/* TODO: check and uncheck and add separator in tool bar */
void TopToolBar::addToolButtons()
{
    QButtonGroup *btnGroup = new QButtonGroup(this);

    QWidget *spacer = new QWidget(this);
    spacer->setFixedWidth(60);
    addWidget(spacer);

    QString style;
    for(int i=0; i<8; i++){
        QToolButton *toolBtn = new QToolButton(this);
        style = "QToolButton{border-image: url(:/images/toolbtn";
        style += QString::number(i);
        style += ");} QToolButton:pressed{margin-top: 2px; margin-left: 2px; margin-right: 2px; margin-bottom: 2px;}";
        toolBtn->setStyleSheet(style);
        toolBtn->setFixedSize(40, 40);
        btnGroup->addButton(toolBtn, i);
        addWidget(toolBtn);

        QWidget *spacer = new QWidget(this);
        spacer->setFixedWidth(50);
        addWidget(spacer);
    }

    QToolButton *onionBtn = new QToolButton(this);//TODO: a bug in clearing the onion
    style = "QToolButton{border-image: url(:/images/onion);} QToolButton:checked{border-image: url(:/images/onion_on);}";
    onionBtn->setStyleSheet(style);
    onionBtn->setFixedSize(40, 40);
    onionBtn->setCheckable(true);
    btnGroup->addButton(onionBtn, 8);
    addWidget(onionBtn);

    QObject::connect(btnGroup, SIGNAL(buttonClicked(int)),
                     this, SLOT(emitWhichTopToolBarBtnWasClickedSignal(int)));
}

/* the slot functions */
void TopToolBar::emitWhichTopToolBarBtnWasClickedSignal(int btnID)
{
    emit m_signalBox->whichTopToolBarBtnWasClicked(btnID);
}

/* implement the TopToolBarInterface plugin interface functions */
TopSignalBox *TopToolBar::getSignalBox()
{
    return m_signalBox;
}

void TopToolBar::install(QMainWindow *mainWindow)
{
    setParent(mainWindow);
    setMovable(false);
    mainWindow->addToolBar(Qt::TopToolBarArea, this);
}
