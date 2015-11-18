#ifndef LEFTTOOLBAR_H
#define LEFTTOOLBAR_H

#include <QObject>
#include <QToolBar>
#include <QWidget>
#include <QActionGroup>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAction>
#include <QString>
#include <QColor>
#include "materiallib.h"

#include <QtPlugin>
#include "interfaces.h"

class LeftToolBar : public QToolBar, public LeftToolBarInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID LeftToolBarInterface_iid FILE "lefttoolbar.json")
    Q_INTERFACES(LeftToolBarInterface)

public:
    explicit LeftToolBar(QWidget *parent = 0);

    LeftSignalBox *getSignalBox() Q_DECL_OVERRIDE;
    void install(QMainWindow *mainWindow) Q_DECL_OVERRIDE;

private slots:
    void emitWhichToolsBoardBtnWasClickedSignal(QAction *checkedAction);
    void emitPenWidthChangedSignal(int newWidth);
    void emitPenFeatherChangedSignal(int newFeather);
    void emitColorChangedSignal(QColor newColor);
    void emitMaterialImagePathChangedSignal(int libBtnID);
    void shiftToToolsLib();
    void shiftToMaterialLib();

private:
    void divideToolBar();
    QVBoxLayout *getLeftVLayout();
    void addDrawingToolsBoard(QActionGroup *group, QVBoxLayout *leftVLayout);
    QActionGroup *getDrawingToolsActionGroup();
    QAction *createCheckableAction(QString iconURL, bool isChecked = false);
    QVBoxLayout *getRightVLayout();
    QHBoxLayout *getSubBottomHLayout();
    void addShiftButtonsInBottom(QHBoxLayout *bottomHLayout);
    void addCartoonButtonsInBottom(QVBoxLayout *bottomInnerVLayout);

    LeftSignalBox *m_signalBox;
    QWidget *m_toolsLibWidget;
    MaterialLib *m_materialLibWidget;
};

#endif // LEFTTOOLBAR_H
