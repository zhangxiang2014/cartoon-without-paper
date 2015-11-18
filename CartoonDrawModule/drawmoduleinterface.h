#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>
#include <QMainWindow>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>
#include <QtUiPlugin/QDesignerCustomWidgetCollectionInterface>

class LoadDrawInterface
{
public:
    virtual ~LoadDrawInterface() {}
    virtual void LoadUI() = 0;
    virtual void setBottomUI(QWidget* videoUI) = 0;
};

#define LoadDrawInterface_iid "org.DrawModuleInterface.LoadDrawInterface"

Q_DECLARE_INTERFACE(LoadDrawInterface, LoadDrawInterface_iid)

#endif // CUSTOMWIDGET_H


