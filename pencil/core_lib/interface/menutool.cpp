
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
#include "menutool.h"


// ----------------------------------------------------------------------------------

MenuToolWidget::MenuToolWidget(const QString title, QWidget* pParent ) :
    BaseDockWidget( title, pParent, Qt::Tool )
{
    setWindowFlags(Qt::FramelessWindowHint);     //也是去掉标题栏的语句
    setTitleBarWidget(new QWidget);
    QFrame* toolGroup = new QFrame();
    toolGroup->setStyleSheet("background-image: url(:icons/b2.jpg)");
    setWidget(toolGroup);

    QGridLayout* layout = new QGridLayout();

    openButton = newToolButton(QPixmap(":icons/open.png"), tr("Pencil Tool <b>(N)</b>: Sketch with pencil"));
    saveButton = newToolButton(QPixmap(":icons/save.png"), tr("Select Tool <b>(V)</b>: Select an object"));
    loadButton = newToolButton(QPixmap(":icons/mirrorV.png"), tr("Move Tool <b>(Q)</b>: Move an object"));
    cutButton = newToolButton(QPixmap(":icons/cut.png"), tr("Hand Tool <b>(H)</b>: Move the canvas"));
    copyButton = newToolButton(QPixmap(":icons/copy.png"), tr("Pen Tool <b>(P)</b>: Sketch with pen"));
    pasteButton = newToolButton(QPixmap(":icons/paste.png"), tr("Eraser Tool <b>(E)</b>: Erase"));
    undoButton = newToolButton(QPixmap(":icons/undo.png"), tr("Polyline Tool <b>(Y)</b>: Create line/curves"));
    recycleButton = newToolButton(QPixmap(":icons/redo.png"), tr("Paint Bucket Tool <b>(K)</b>: Fill selected area with a color"));
    onionupButton = newToolButton(QPixmap(":icons/onionPrev.png"), tr("Brush Tool <b>(B)</b>: Paint smooth stroke with a brush"));
    oniondownButton = newToolButton(QPixmap(":icons/onionNext.png"), tr("Eyedropper Tool <b>(I)</b>: Set color from the stage<br><b>[ALT]</b> for instant access"));


//    pencilButton->setWhatsThis(tr("Pencil Tool (N)"));
//    selectButton->setWhatsThis(tr("Select Tool (V)"));
//    moveButton->setWhatsThis(tr("Move Tool (Q)"));
//    handButton->setWhatsThis(tr("Hand Tool (H)"));
//    penButton->setWhatsThis(tr("Pen Tool (P)"));
//    eraserButton->setWhatsThis(tr("Eraser Tool (E)"));
//    polylineButton->setWhatsThis(tr("Polyline Tool (Y)"));
//    bucketButton->setWhatsThis(tr("Paint Bucket Tool(K)"));
//    colouringButton->setWhatsThis(tr("Brush Tool(B)"));
//    eyedropperButton->setWhatsThis(tr("Eyedropper Tool (I)"));
//    clearButton->setWhatsThis(tr("Clear Tool"));
//    smudgeButton->setWhatsThis(tr("Smudge Tool (A)"));

//    pencilButton->setCheckable(true);
//    penButton->setCheckable(true);
//    polylineButton->setCheckable(true);
//    bucketButton->setCheckable(true);
//    colouringButton->setCheckable(true);
//    smudgeButton->setCheckable(true);
//    eyedropperButton->setCheckable(true);
//    selectButton->setCheckable(true);
//    moveButton->setCheckable(true);
//    handButton->setCheckable(true);
//    eraserButton->setCheckable(true);
//    pencilButton->setChecked(true);

    layout->setMargin(10);
    layout->setSpacing(10);

    layout->addWidget(openButton,0,0);
    //layout->setAlignment(moveButton, Qt::AlignRight);
    layout->addWidget(saveButton,0,1);
    //layout->setAlignment(clearButton, Qt::AlignLeft);

    layout->addWidget(loadButton,0,2);
    //layout->setAlignment(selectButton, Qt::AlignRight);
    layout->addWidget(cutButton,0,3);
    //layout->setAlignment(colouringButton, Qt::AlignLeft);

    layout->addWidget(copyButton,0,4);
    //layout->setAlignment(polylineButton, Qt::AlignRight);
    layout->addWidget(pasteButton,0,5);
    //layout->setAlignment(smudgeButton, Qt::AlignLeft);

    layout->addWidget(undoButton,0,6);
    //layout->setAlignment(penButton, Qt::AlignRight);
    layout->addWidget(recycleButton,0,7);
    //layout->setAlignment(handButton, Qt::AlignLeft);

    layout->addWidget(onionupButton,0,8);
    //layout->setAlignment(pencilButton, Qt::AlignRight);
    layout->addWidget(oniondownButton,0,9);
    //layout->setAlignment(bucketButton, Qt::AlignLeft);


    toolGroup->setLayout(layout);
    toolGroup->setMaximumHeight(70);

    this->setMaximumHeight(70);


}
void MenuToolWidget::initUI()
{

}

void MenuToolWidget::updateUI()
{

}


QToolButton* MenuToolWidget::newToolButton(const QPixmap& icon, QString strToolTip)
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




