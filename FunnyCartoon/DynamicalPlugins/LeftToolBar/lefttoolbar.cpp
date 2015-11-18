#include "lefttoolbar.h"
#include "penshapeboard.h"
#include "colorbar.h"
#include <QIcon>
#include <QToolButton>
#include <QDir>
#include <QDebug>

LeftToolBar::LeftToolBar(QWidget *parent) :
    QToolBar(parent)
{
    m_signalBox = new LeftSignalBox(this);
    divideToolBar();
}

/* divide the tool bar as top and bottom widget */
void LeftToolBar::divideToolBar()
{
    /* add top widget in tool bar */
    QWidget *topWidget = new QWidget(this);
    QVBoxLayout *topVLayout = new QVBoxLayout;

    //add tools library child widget in top widget
    m_toolsLibWidget = new QWidget(topWidget);
    QHBoxLayout *toolsHLayout = new QHBoxLayout;

    QWidget *leftOfToolsWidget = new QWidget(m_toolsLibWidget);//the left inner widget
    leftOfToolsWidget->setLayout(getLeftVLayout());
    toolsHLayout->addWidget(leftOfToolsWidget);

    QWidget *rightOfToolsWidget = new QWidget(m_toolsLibWidget);//the right inner widget
    rightOfToolsWidget->setLayout(getRightVLayout());
    toolsHLayout->addWidget(rightOfToolsWidget);

    m_toolsLibWidget->setLayout(toolsHLayout);
    topVLayout->addWidget(m_toolsLibWidget);

    //add material library child widget in top widget
    m_materialLibWidget = new MaterialLib(topWidget);
    m_materialLibWidget->hide();
    topVLayout->addWidget(m_materialLibWidget);
    for(int i=0; i<m_materialLibWidget->m_libBtnGroupList.size(); i++){
        QObject::connect(m_materialLibWidget->m_libBtnGroupList.at(i),
                         SIGNAL(buttonClicked(int)),
                         this, SLOT(emitMaterialImagePathChangedSignal(int)));
    }

    topWidget->setFixedWidth(250);//to fix the bottom widget
    topWidget->setLayout(topVLayout);
    this->addWidget(topWidget);

    /* add bottom widget in tool bar */
    QWidget *bottomWidget = new QWidget(this);
    QHBoxLayout *bottomHLayout = new QHBoxLayout;

    QWidget *subBottomWidget = new QWidget(bottomWidget);
    subBottomWidget->setLayout(getSubBottomHLayout());
    bottomHLayout->addWidget(subBottomWidget);

    bottomWidget->setLayout(bottomHLayout);
    this->addWidget(bottomWidget);
}

/* add drawing tools board and shape board in the left of tools library widget */
QVBoxLayout *LeftToolBar::getLeftVLayout()
{
    QVBoxLayout *topWidgetLeftVLayout = new QVBoxLayout;

    addDrawingToolsBoard(getDrawingToolsActionGroup(), topWidgetLeftVLayout);

    //add pen shape board in tool bar
    PenShapeBoard *penShapeBoard = new PenShapeBoard(m_toolsLibWidget);
    topWidgetLeftVLayout->addWidget(penShapeBoard);
    QObject::connect(penShapeBoard, SIGNAL(penShapeWidthChanged(int)),
                     this, SLOT(emitPenWidthChangedSignal(int)));
    QObject::connect(penShapeBoard, SIGNAL(penShapeFeatherChanged(int)),
                     this, SLOT(emitPenFeatherChangedSignal(int)));

    //keep bottom widget in toolbar bottom when scale the window
    QWidget *spacer = new QWidget(m_toolsLibWidget);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    topWidgetLeftVLayout->addWidget(spacer);

    return topWidgetLeftVLayout;
}

void LeftToolBar::addDrawingToolsBoard(QActionGroup *group, QVBoxLayout *leftVLayout)
{
    QList<QAction *> toolsActions = group->actions();

    //set drawing tools button default action and style
    QWidget *toolsBoard = new QWidget(m_toolsLibWidget);
    QVBoxLayout *vToolsBoard = new QVBoxLayout;

    //this loop to create checkable tool button
    for(int i=0; i<4; i++){
        QHBoxLayout *hLayout = new QHBoxLayout;
        for(int j=0; j<2; j++){
            QToolButton *toolBtn = new QToolButton(toolsBoard);
            toolBtn->setDefaultAction(toolsActions[2*i+j]);
            toolBtn->setObjectName("roundToolBtn");
            hLayout->addWidget(toolBtn);
        }
        vToolsBoard->addLayout(hLayout);
    }

    toolsBoard->setLayout(vToolsBoard);
    leftVLayout->addWidget(toolsBoard);
}

QActionGroup *LeftToolBar::getDrawingToolsActionGroup()
{
    //add main tool bar actions in a action group and connect slots
    QActionGroup *toolsBoardGroup = new QActionGroup(this);

    //the loop to create checkable actions
    QString toolIconPath = ":/images/draw_tools/tool0";
    QAction *actionFirst = createCheckableAction(toolIconPath, true);
    toolsBoardGroup->addAction(actionFirst);
    for(int i=1; i<6; i++){
        toolIconPath = ":/images/draw_tools/tool";
        toolIconPath += QString::number(i);
        QAction *checkableAction = createCheckableAction(toolIconPath);
        toolsBoardGroup->addAction(checkableAction);
    }

    //TODO: the loop to create uncheckable actions
    for(int i=6; i<8; i++){
        toolIconPath = ":/images/draw_tools/tool";
        toolIconPath += QString::number(i);
        QAction *uncheckableAction = new QAction(this);
        uncheckableAction->setIcon(QIcon(toolIconPath));
        toolsBoardGroup->addAction(uncheckableAction);
    }

    QObject::connect(toolsBoardGroup, SIGNAL(triggered(QAction*)),
                     this, SLOT(emitWhichToolsBoardBtnWasClickedSignal(QAction *)));

    return toolsBoardGroup;
}

QAction *LeftToolBar::createCheckableAction(QString iconURL, bool isChecked)
{
    QAction *action = new QAction(this);
    action->setIcon(QIcon(iconURL));
    action->setCheckable(true);
    action->setChecked(isChecked);

    return action;
}

/* add color bar in the right of tools library widget */
QVBoxLayout *LeftToolBar::getRightVLayout()
{
    QVBoxLayout *topWidgetRightVLayout = new QVBoxLayout;

    ColorBar *colorBar = new ColorBar(false, m_toolsLibWidget);//add vertical color bar
    colorBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    colorBar->setFixedWidth(ColorBar::SIZE);
    topWidgetRightVLayout->addWidget(colorBar);

    QObject::connect(colorBar, SIGNAL(currentColorChanged(QColor)),
                     this, SLOT(emitColorChangedSignal(QColor)));

    return topWidgetRightVLayout;
}

/* set the bottom widget of the tool bar */
QHBoxLayout *LeftToolBar::getSubBottomHLayout()
{
    QHBoxLayout *bottomHLayout = new QHBoxLayout;
    QVBoxLayout *bottomInnerVLayout = new QVBoxLayout;

    addShiftButtonsInBottom(bottomHLayout);
    addCartoonButtonsInBottom(bottomInnerVLayout);
    bottomHLayout->addLayout(bottomInnerVLayout);
    return bottomHLayout;
}

void LeftToolBar::addShiftButtonsInBottom(QHBoxLayout *bottomHLayout)
{
    QActionGroup *shiftButtonGroup = new QActionGroup(this);
    QToolButton *toolsLibBtn = new QToolButton;
    QToolButton *materialLibBtn = new QToolButton;

    //the shift button of drawing tools
    QAction *actionToolsLib = createCheckableAction(":/images/draw_lib", true);
    shiftButtonGroup->addAction(actionToolsLib);
    toolsLibBtn->setDefaultAction(actionToolsLib);
    toolsLibBtn->setObjectName("roundToolBtn");
    bottomHLayout->addWidget(toolsLibBtn);

    //the shift button of material library
    QAction *actionMaterialLib = createCheckableAction(":/images/material_lib");
    shiftButtonGroup->addAction(actionMaterialLib);
    materialLibBtn->setDefaultAction(actionMaterialLib);
    materialLibBtn->setObjectName("roundToolBtn");
    bottomHLayout->addWidget(materialLibBtn);

    //connect shift libraries functions
    QObject::connect(actionToolsLib, SIGNAL(triggered(bool)),
                     this, SLOT(shiftToToolsLib()));
    QObject::connect(actionMaterialLib, SIGNAL(triggered(bool)),
                     this, SLOT(shiftToMaterialLib()));
}

void LeftToolBar::addCartoonButtonsInBottom(QVBoxLayout *bottomInnerVLayout)
{
    QToolButton *plusBtn = new QToolButton;
    QToolButton *minusBtn = new QToolButton;
    QToolButton *copyBtn = new QToolButton;

    plusBtn->setObjectName("addAFrameBtn");//the plus button
    bottomInnerVLayout->addWidget(plusBtn);
    minusBtn->setObjectName("subAFrameBtn");//the minus button
    bottomInnerVLayout->addWidget(minusBtn);
    copyBtn->setObjectName("copyAFrameBtn");//the box button
    bottomInnerVLayout->addWidget(copyBtn);
}

/* the slot functions */
void LeftToolBar::emitWhichToolsBoardBtnWasClickedSignal(QAction *checkedAction)
{
    QActionGroup *actionGroup = qobject_cast<QActionGroup *>(sender());
    int btnID = actionGroup->actions().indexOf(checkedAction);
    if(btnID != -1){
        emit m_signalBox->whichToolsBoardBtnWasClicked(btnID);
    }
}

void LeftToolBar::emitPenWidthChangedSignal(int newWidth)
{
    emit m_signalBox->penWidthChanged(newWidth);
}

void LeftToolBar::emitPenFeatherChangedSignal(int newFeather)
{
    emit m_signalBox->penFeatherChanged(newFeather);
}

void LeftToolBar::emitColorChangedSignal(QColor newColor)
{
    emit m_signalBox->colorChanged(newColor);
}

void LeftToolBar::emitMaterialImagePathChangedSignal(int libBtnID)
{
    QString newImgPath = QDir::currentPath();
    newImgPath += "/";
    newImgPath += m_materialLibWidget->m_currentLibPath;
    newImgPath += "/";
    newImgPath += QString::number(libBtnID+1);
    newImgPath += ".png";

    emit m_signalBox->materialImagePathChanged(newImgPath);
}

void LeftToolBar::shiftToToolsLib()
{
    m_materialLibWidget->hide();
    m_toolsLibWidget->show();
}

void LeftToolBar::shiftToMaterialLib()
{
    m_toolsLibWidget->hide();
    m_materialLibWidget->show();
}

/* implement the LeftToolBarInterface plugin interface functions */
LeftSignalBox *LeftToolBar::getSignalBox()
{
    return m_signalBox;
}

void LeftToolBar::install(QMainWindow *mainWindow)
{
    setParent(mainWindow);
    setMovable(false);
    mainWindow->addToolBar(Qt::LeftToolBarArea, this);
}
