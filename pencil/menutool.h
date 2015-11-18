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
#ifndef MENUTOOLWIDGET_H
#define MENUTOOLWIDGET_H
#include "basedockwidget.h"
#include <QToolButton>
#include <QIcon>

class QToolButton;
class QIcon;

class MenuToolWidget : public BaseDockWidget
{
    Q_OBJECT

public:
    explicit MenuToolWidget(const QString title, QWidget* pParent );
    void initUI() override;
    void updateUI() override;

//    void setCurrentTool( ToolType );

//public slots:
//    void pencilOn();
//    void eraserOn();
//    void selectOn();
//    void moveOn();
//    void penOn();
//    void handOn();
//    void polylineOn();
//    void bucketOn();
//    void eyedropperOn();
//    void brushOn();
//    void smudgeOn();

//signals:
//    void clearButtonClicked();

private:
    QToolButton* newToolButton(const QIcon&, QString);
    QToolButton* newToolButton(const QPixmap& icon, QString strToolTip);
  //  void deselectAllTools();

    QToolButton* openButton;
    QToolButton* saveButton;
    QToolButton* loadButton;
    QToolButton* cutButton;
    QToolButton* copyButton;
    QToolButton* pasteButton;
    QToolButton* undoButton;
    QToolButton* recycleButton;
    QToolButton* onionupButton;
    QToolButton* oniondownButton;
//    QToolButton* smudgeButton;
//    QToolButton* clearButton;
};

#endif
