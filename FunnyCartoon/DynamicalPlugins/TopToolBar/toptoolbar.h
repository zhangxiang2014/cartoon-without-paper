#ifndef TOPTOOLBAR_H
#define TOPTOOLBAR_H

#include <QObject>
#include <QToolBar>
#include <QWidget>

#include <QtPlugin>
#include "interfaces.h"

class TopToolBar : public QToolBar, public TopToolBarInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID TopToolBarInterface_iid FILE "toptoolbar.json")
    Q_INTERFACES(TopToolBarInterface)

public:
    explicit TopToolBar(QWidget *parent = 0);

    TopSignalBox *getSignalBox() Q_DECL_OVERRIDE;
    void install(QMainWindow *mainWindow) Q_DECL_OVERRIDE;

private slots:
    void emitWhichTopToolBarBtnWasClickedSignal(int btnID);

private:
    void addToolButtons();

    TopSignalBox *m_signalBox;
};

#endif // TOPTOOLBAR_H
