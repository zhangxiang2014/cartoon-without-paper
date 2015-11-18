/*

Pencil - Traditional Animation Software
Copyright (C) 2005-2007 Patrick Corrieri & Pascal Naidon

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation;

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/

#include <cmath>

#include <QFrame>
#include <QToolButton>
#include <QGridLayout>
#include <QBitmap>

#include "spinslider.h"
#include "tooloptiondockwidget.h"
#include "toolbox.h"
#include "editor.h"
#include "toolmanager.h"

// ----------------------------------------------------------------------------------

ToolBoxWidget::ToolBoxWidget(const QString title, QWidget* pParent ) :
    BaseDockWidget( title, pParent, Qt::Tool )
{
    setWindowFlags(Qt::FramelessWindowHint);     //也是去掉标题栏的语句
    setTitleBarWidget(new QWidget);
    QFrame* toolGroup = new QFrame();
    toolGroup->setStyleSheet("background-image: url(:icons/b2.jpg)");
    setWidget(toolGroup);

    QGridLayout* layout = new QGridLayout();

    pencilButton = newToolButton(QPixmap("://icons/pencil2.png"), tr("Pencil Tool <b>(N)</b>: Sketch with pencil"));
    selectButton = newToolButton(QPixmap(":icons/select.png"), tr("Select Tool <b>(V)</b>: Select an object"));
    moveButton = newToolButton(QPixmap(":icons/arrow.png"), tr("Move Tool <b>(Q)</b>: Move an object"));
    handButton = newToolButton(QPixmap(":icons/onion-red.png"), tr("Hand Tool <b>(H)</b>: Move the canvas"));
    penButton = newToolButton(QPixmap(":icons/pen.png"), tr("Pen Tool <b>(P)</b>: Sketch with pen"));
    eraserButton = newToolButton(QPixmap(":icons/eraser.png"), tr("Eraser Tool <b>(E)</b>: Erase"));
    polylineButton = newToolButton(QPixmap(":icons/polyline.png"), tr("Polyline Tool <b>(Y)</b>: Create line/curves"));
    bucketButton = newToolButton(QPixmap(":icons/bucket.png"), tr("Paint Bucket Tool <b>(K)</b>: Fill selected area with a color"));
    colouringButton = newToolButton(QPixmap(":icons/brush.png"), tr("Brush Tool <b>(B)</b>: Paint smooth stroke with a brush"));
    eyedropperButton = newToolButton(QPixmap(":icons/eyedropper.png"), tr("Eyedropper Tool <b>(I)</b>: Set color from the stage<br><b>[ALT]</b> for instant access"));
    clearButton = newToolButton(QPixmap(":icons/clear.png"), tr("Clear Frame <b>(Ctrl+D)</b>: Erases content of selected frame"));
    smudgeButton = newToolButton(QPixmap(":icons/smudge.png"), tr("Smudge Tool <b>(A)</b>:<br>Edit polyline/curves<br>Liquify bitmap pixels<br><b>+[Alt]</b>: Smooth"));

    pencilButton->setWhatsThis(tr("Pencil Tool (N)"));
    selectButton->setWhatsThis(tr("Select Tool (V)"));
    moveButton->setWhatsThis(tr("Move Tool (Q)"));
    handButton->setWhatsThis(tr("Hand Tool (H)"));
    penButton->setWhatsThis(tr("Pen Tool (P)"));
    eraserButton->setWhatsThis(tr("Eraser Tool (E)"));
    polylineButton->setWhatsThis(tr("Polyline Tool (Y)"));
    bucketButton->setWhatsThis(tr("Paint Bucket Tool(K)"));
    colouringButton->setWhatsThis(tr("Brush Tool(B)"));
    eyedropperButton->setWhatsThis(tr("Eyedropper Tool (I)"));
    clearButton->setWhatsThis(tr("Clear Tool"));
    smudgeButton->setWhatsThis(tr("Smudge Tool (A)"));

    pencilButton->setCheckable(true);
    penButton->setCheckable(true);
    polylineButton->setCheckable(true);
    bucketButton->setCheckable(true);
    colouringButton->setCheckable(true);
    smudgeButton->setCheckable(true);
    eyedropperButton->setCheckable(true);
    selectButton->setCheckable(true);
    moveButton->setCheckable(true);
    handButton->setCheckable(true);
    eraserButton->setCheckable(true);
    pencilButton->setChecked(true);

    layout->setMargin(10);
    layout->setSpacing(2);

    layout->addWidget(moveButton,0,0);
    layout->setAlignment(moveButton, Qt::AlignRight);
    layout->addWidget(clearButton,0,1);
    layout->setAlignment(clearButton, Qt::AlignLeft);

    layout->addWidget(selectButton,1,0);
    layout->setAlignment(selectButton, Qt::AlignRight);
    layout->addWidget(colouringButton,1,1);
    layout->setAlignment(colouringButton, Qt::AlignLeft);

    layout->addWidget(polylineButton,2,0);
    layout->setAlignment(polylineButton, Qt::AlignRight);
    layout->addWidget(smudgeButton,2,1);
    layout->setAlignment(smudgeButton, Qt::AlignLeft);

    layout->addWidget(penButton,3,0);
    layout->setAlignment(penButton, Qt::AlignRight);
    layout->addWidget(handButton,3,1);
    layout->setAlignment(handButton, Qt::AlignLeft);

    layout->addWidget(pencilButton,4,0);
    layout->setAlignment(pencilButton, Qt::AlignRight);
    layout->addWidget(bucketButton,4,1);
    layout->setAlignment(bucketButton, Qt::AlignLeft);

    layout->addWidget(eyedropperButton,5,0);
    layout->setAlignment(eyedropperButton, Qt::AlignRight);
    layout->addWidget(eraserButton,5,1);
    layout->setAlignment(eraserButton, Qt::AlignLeft);

    toolGroup->setLayout(layout);
    //toolGroup->setMaximumHeight(6*32+1+200+200);
    toolGroup->setMaximumHeight(400);
    this->setMaximumHeight(500);

    connect(pencilButton, SIGNAL(clicked()), this, SLOT(pencilOn()));
    connect(eraserButton, SIGNAL(clicked()), this, SLOT(eraserOn()));
    connect(selectButton, SIGNAL(clicked()), this, SLOT(selectOn()));
    connect(moveButton, SIGNAL(clicked()), this, SLOT(moveOn()));
    connect(penButton, SIGNAL(clicked()), this, SLOT(penOn()));
   // connect(handButton, SIGNAL(clicked()), this, SLOT(handOn()));
    connect(polylineButton, SIGNAL(clicked()), this, SLOT(polylineOn()));
    connect(bucketButton, SIGNAL(clicked()), this, SLOT(bucketOn()));
    connect(eyedropperButton, SIGNAL(clicked()), this, SLOT(eyedropperOn()));
    connect(colouringButton, SIGNAL(clicked()), this, SLOT(brushOn()));
    connect(smudgeButton, SIGNAL(clicked()), this, SLOT(smudgeOn()));

    // pass to editor
    connect(clearButton, SIGNAL(clicked()), this, SIGNAL(clearButtonClicked()));
}

void ToolBoxWidget::initUI()
{

}

void ToolBoxWidget::updateUI()
{

}

QToolButton* ToolBoxWidget::newToolButton(const QPixmap& icon, QString strToolTip)
{
    QToolButton* toolButton = new QToolButton(this);
    toolButton->setAutoRaise(true);
    toolButton->setIconSize( QSize(55,55) );
    toolButton->setFixedSize(55, 55);
    toolButton->setIcon(icon);
    toolButton->setStyleSheet("background:transparent;");
   // toolButton->setMask(icon.createHeuristicMask());
    toolButton->setToolTip(strToolTip);
//    toolButton->setStyleSheet("QToolButton{min-height:20;border-style:solid;border-top-left-radius:2px;"
//                              "border-top-right-radius:2px;""border:2px groove gray;border-radius:10px;padding:2px 4px;"
//                              "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop:0 rgb(226,236,241), "
//                              "stop: 0.2 rgb(233,242,247),stop: 0.7 rgb(176,208,225),"
//                              "stop: 0.8 rgb(176,208,225),"
//                              "stop: 1 rgb(192,216,232));}");

    return toolButton;
}

void ToolBoxWidget::pencilOn()
{
    editor()->tools()->setCurrentTool( PENCIL );

    deselectAllTools();
    pencilButton->setChecked(true);
}

void ToolBoxWidget::eraserOn()
{
    editor()->tools()->setCurrentTool( ERASER );

    deselectAllTools();
    eraserButton->setChecked(true);
}

void ToolBoxWidget::selectOn()
{
    editor()->tools()->setCurrentTool( SELECT );

    deselectAllTools();
    selectButton->setChecked(true);
}

void ToolBoxWidget::moveOn()
{
    editor()->tools()->setCurrentTool( MOVE );

    deselectAllTools();
    moveButton->setChecked(true);
}

void ToolBoxWidget::penOn()
{
    editor()->tools()->setCurrentTool( PEN );

    deselectAllTools();
    penButton->setChecked(true);
}

void ToolBoxWidget::handOn()
{
    editor()->tools()->setCurrentTool( HAND );
    
    deselectAllTools();
    handButton->setChecked(true);
}

void ToolBoxWidget::polylineOn()
{
    editor()->tools()->setCurrentTool( POLYLINE );

    deselectAllTools();
    polylineButton->setChecked(true);
}

void ToolBoxWidget::bucketOn()
{
    editor()->tools()->setCurrentTool( BUCKET );

    deselectAllTools();
    bucketButton->setChecked(true);
}

void ToolBoxWidget::eyedropperOn()
{
    editor()->tools()->setCurrentTool( EYEDROPPER );

    deselectAllTools();
    eyedropperButton->setChecked(true);
}

void ToolBoxWidget::brushOn()
{
    editor()->tools()->setCurrentTool( BRUSH );
  
    deselectAllTools();
    colouringButton->setChecked(true);
}

void ToolBoxWidget::smudgeOn()
{
    editor()->tools()->setCurrentTool( SMUDGE );

    deselectAllTools();
    smudgeButton->setChecked(true);
}

void ToolBoxWidget::deselectAllTools()
{
    pencilButton->setChecked(false);
    eraserButton->setChecked(false);
    selectButton->setChecked(false);
    moveButton->setChecked(false);
    handButton->setChecked(false);
    penButton->setChecked(false);
    polylineButton->setChecked(false);
    bucketButton->setChecked(false);
    eyedropperButton->setChecked(false);
    colouringButton->setChecked(false);
    smudgeButton->setChecked(false);
}

void ToolBoxWidget::setCurrentTool( ToolType toolType )
{
    switch(toolType)
    {
    case PENCIL:
        emit pencilOn();
        break;
    case ERASER:
        emit eraserOn();
        break;
    case SELECT:
        emit selectOn();
        break;
    case MOVE:
        emit moveOn();
        break;
    case HAND:
        emit handOn();
        break;
    case SMUDGE:
        emit smudgeOn();
        break;
    case PEN:
        emit penOn();
        break;
    case POLYLINE:
        emit polylineOn();
        break;
    case BUCKET:
        emit bucketOn();
        break;
    case EYEDROPPER:
        emit eyedropperOn();
        break;
    case BRUSH:
        emit brushOn();
        break;
    default:
        break;
    }
}
