/*

Pencil - Traditional Animation Software
Copyright (C) 2005-2007 Patrick Corrieri & Pascal Naidon
Copyright (C) 2013-2014 Matt Chiawen Chang

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation;

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/

#include "timeline.h"

#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QScrollBar>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>
#include <QSplitter>
#include <QMessageBox>
#include <QLabel>

#include "layer.h"
#include "editor.h"
#include "layermanager.h"
#include "toolbox.h"
#include "timecontrols.h"
#include "timelinecells.h"

//ToolBoxWidget::ToolBoxWidget(const QString title, QWidget* pParent ) :
//    BaseDockWidget( title, pParent, Qt::Tool )
//{
//    QFrame* toolGroup = new QFrame();
//    toolGroup->setStyleSheet("background-image: url(:icons/b2.jpg)");
//    setWidget(toolGroup);

//    QGridLayout* layout = new QGridLayout();

//    pencilButton = newToolButton(QPixmap("://icons/pencil2.png"), tr("Pencil Tool <b>(N)</b>: Sketch with pencil"));
//    selectButton = newToolButton(QPixmap(":icons/select.png"), tr("Select Tool <b>(V)</b>: Select an object"));
//    moveButton = newToolButton(QPixmap(":icons/arrow.png"), tr("Move Tool <b>(Q)</b>: Move an object"));
//    handButton = newToolButton(QPixmap(":icons/test1.png"), tr("Hand Tool <b>(H)</b>: Move the canvas"));
//    penButton = newToolButton(QPixmap(":icons/pen.png"), tr("Pen Tool <b>(P)</b>: Sketch with pen"));
//    eraserButton = newToolButton(QPixmap(":icons/eraser.png"), tr("Eraser Tool <b>(E)</b>: Erase"));
//    polylineButton = newToolButton(QPixmap(":icons/polyline.png"), tr("Polyline Tool <b>(Y)</b>: Create line/curves"));
//    bucketButton = newToolButton(QPixmap(":icons/bucket.png"), tr("Paint Bucket Tool <b>(K)</b>: Fill selected area with a color"));
//    colouringButton = newToolButton(QPixmap(":icons/brush.png"), tr("Brush Tool <b>(B)</b>: Paint smooth stroke with a brush"));
//    eyedropperButton = newToolButton(QPixmap(":icons/eyedropper.png"), tr("Eyedropper Tool <b>(I)</b>: Set color from the stage<br><b>[ALT]</b> for instant access"));
//    clearButton = newToolButton(QPixmap(":icons/clear.png"), tr("Clear Frame <b>(Ctrl+D)</b>: Erases content of selected frame"));
//    smudgeButton = newToolButton(QPixmap(":icons/smudge.png"), tr("Smudge Tool <b>(A)</b>:<br>Edit polyline/curves<br>Liquify bitmap pixels<br><b>+[Alt]</b>: Smooth"));

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

//    layout->setMargin(10);
//    layout->setSpacing(10);

//    layout->addWidget(moveButton,0,0);
//    layout->setAlignment(moveButton, Qt::AlignRight);
//    layout->addWidget(clearButton,0,1);
//    layout->setAlignment(clearButton, Qt::AlignLeft);

//    layout->addWidget(selectButton,1,0);
//    layout->setAlignment(selectButton, Qt::AlignRight);
//    layout->addWidget(colouringButton,1,1);
//    layout->setAlignment(colouringButton, Qt::AlignLeft);

//    layout->addWidget(polylineButton,2,0);
//    layout->setAlignment(polylineButton, Qt::AlignRight);
//    layout->addWidget(smudgeButton,2,1);
//    layout->setAlignment(smudgeButton, Qt::AlignLeft);

//    layout->addWidget(penButton,3,0);
//    layout->setAlignment(penButton, Qt::AlignRight);
//    layout->addWidget(handButton,3,1);
//    layout->setAlignment(handButton, Qt::AlignLeft);

//    layout->addWidget(pencilButton,4,0);
//    layout->setAlignment(pencilButton, Qt::AlignRight);
//    layout->addWidget(bucketButton,4,1);
//    layout->setAlignment(bucketButton, Qt::AlignLeft);

//    layout->addWidget(eyedropperButton,5,0);
//    layout->setAlignment(eyedropperButton, Qt::AlignRight);
//    layout->addWidget(eraserButton,5,1);
//    layout->setAlignment(eraserButton, Qt::AlignLeft);

//    toolGroup->setLayout(layout);
//    toolGroup->setMaximumHeight(6*32+1+200+200);

//    this->setMaximumHeight(800);

//    connect(pencilButton, SIGNAL(clicked()), this, SLOT(pencilOn()));
//    connect(eraserButton, SIGNAL(clicked()), this, SLOT(eraserOn()));
//    connect(selectButton, SIGNAL(clicked()), this, SLOT(selectOn()));
//    connect(moveButton, SIGNAL(clicked()), this, SLOT(moveOn()));
//    connect(penButton, SIGNAL(clicked()), this, SLOT(penOn()));
//    connect(handButton, SIGNAL(clicked()), this, SLOT(handOn()));
//    connect(polylineButton, SIGNAL(clicked()), this, SLOT(polylineOn()));
//    connect(bucketButton, SIGNAL(clicked()), this, SLOT(bucketOn()));
//    connect(eyedropperButton, SIGNAL(clicked()), this, SLOT(eyedropperOn()));
//    connect(colouringButton, SIGNAL(clicked()), this, SLOT(brushOn()));
//    connect(smudgeButton, SIGNAL(clicked()), this, SLOT(smudgeOn()));

//    // pass to editor
//    connect(clearButton, SIGNAL(clicked()), this, SIGNAL(clearButtonClicked()));
//}


//void ToolBoxWidget::initUI()
//{

//}

//void ToolBoxWidget::updateUI()
//{

//}

//QToolButton* ToolBoxWidget::newToolButton(const QPixmap& icon, QString strToolTip)
//{
//    QToolButton* toolButton = new QToolButton(this);
//    toolButton->setAutoRaise(true);
//    toolButton->setIconSize( QSize(60,60) );
//    toolButton->setFixedSize(60, 60);
//    toolButton->setIcon(icon);
//    toolButton->setStyleSheet("background:transparent;");
//   // toolButton->setMask(icon.createHeuristicMask());
//    toolButton->setToolTip(strToolTip);
////    toolButton->setStyleSheet("QToolButton{min-height:20;border-style:solid;border-top-left-radius:2px;"
////                              "border-top-right-radius:2px;""border:2px groove gray;border-radius:10px;padding:2px 4px;"
////                              "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop:0 rgb(226,236,241), "
////                              "stop: 0.2 rgb(233,242,247),stop: 0.7 rgb(176,208,225),"
////                              "stop: 0.8 rgb(176,208,225),"
////                              "stop: 1 rgb(192,216,232));}");

//    return toolButton;
//}
TimeLine::TimeLine( QWidget* parent ) : BaseDockWidget( parent, Qt::Tool )
{
}

void TimeLine::initUI()
{
    Q_ASSERT( editor() != nullptr );

    setWindowTitle( "Timeline" );
      setTitleBarWidget(new QWidget);
    QWidget* timeLineContent = new QWidget( this );

    connect( editor(), &Editor::currentFrameChanged, this, &TimeLine::updateFrame );

    mLayerList = new TimeLineCells( this, editor(), TIMELINE_CELL_TYPE::Layers );
    mTracks = new TimeLineCells( this, editor(), TIMELINE_CELL_TYPE::Tracks );

    connect( mLayerList, &TimeLineCells::mouseMovedY, mLayerList, &TimeLineCells::setMouseMoveY );
    connect( mLayerList, &TimeLineCells::mouseMovedY, mTracks,    &TimeLineCells::setMouseMoveY );

    hScrollBar = new QScrollBar( Qt::Horizontal );
    vScrollBar = new QScrollBar( Qt::Vertical );
    vScrollBar->setMinimum( 0 );
    vScrollBar->setMaximum( 1 );
    vScrollBar->setPageStep( 1 );
    updateLength( getFrameLength() );

    //swtich
    QWidget* switchWidget = new QWidget();
    switchWidget->setMinimumWidth(100);
//    QFrame* toolGroup = new QFrame();
//    toolGroup->setStyleSheet("background-image: url(:icons/b2.jpg)");
//    setWidget(toolGroup);
    switchWidget->setStyleSheet("background-image: url(:icons/b2.jpg)");


        QToolButton* switchButton1 = new QToolButton(this);
        switchButton1->setAutoRaise(true);
        switchButton1->setIconSize( QSize(60,60) );
        switchButton1->setFixedSize(60, 60);
        switchButton1->setIcon(QPixmap(":icons/test1.png"));
        switchButton1->setStyleSheet("background:transparent;");

        QToolButton* switchButton2 = new QToolButton(this);
        switchButton2->setAutoRaise(true);
        switchButton2->setIconSize( QSize(60,60) );
        switchButton2->setFixedSize(60, 60);
        switchButton2->setIcon(QPixmap(":icons/test2.png"));
        switchButton2->setStyleSheet("background:transparent;");

    QGridLayout* layout = new QGridLayout();
    layout->setMargin(10);
    layout->setSpacing(2);
    //layout->setContentsMargins(30,0,0,0);

    layout->addWidget(switchButton1,0,0);
    //layout->addWidget(switchButton1);
    layout->setAlignment(switchButton1, Qt::AlignRight);
    layout->addWidget(switchButton2,0,1);
    //layout->addWidget(switchButton2);
    layout->setAlignment(switchButton2, Qt::AlignLeft);
    switchWidget->setLayout(layout);

    //left and right
    QWidget* leftWidget = new QWidget();
    leftWidget->setMinimumWidth( 120 );
    QWidget* rightWidget = new QWidget();


    QWidget* leftToolBar = new QWidget();
    leftToolBar->setFixedHeight( 31 );
    QWidget* rightToolBar = new QWidget();
    rightToolBar->setFixedHeight( 31 );

    // --- left widget ---
    // --------- layer buttons ---------
    QToolBar* layerButtons = new QToolBar( this );
    QLabel* layerLabel = new QLabel( tr( "Layers:" ) );
    layerLabel->setIndent( 5 );
    layerLabel->setFont( QFont( "Helvetica", 10 ) );

    QToolButton* addLayerButton = new QToolButton( this );
    addLayerButton->setIcon( QIcon( ":icons/add.png" ) );
    addLayerButton->setToolTip( "Add Layer" );
    addLayerButton->setFixedSize( 24, 24 );

    QToolButton* removeLayerButton = new QToolButton( this );
    removeLayerButton->setIcon( QIcon( ":icons/remove.png" ) );
    removeLayerButton->setToolTip( "Remove Layer" );
    removeLayerButton->setFixedSize( 24, 24 );

    layerButtons->addWidget( layerLabel );
    layerButtons->addWidget( addLayerButton );
    layerButtons->addWidget( removeLayerButton );

    QHBoxLayout* leftToolBarLayout = new QHBoxLayout();
    leftToolBarLayout->setAlignment( Qt::AlignLeft );
    leftToolBarLayout->setMargin( 0 );
    leftToolBarLayout->addWidget( layerButtons );
    leftToolBar->setLayout( leftToolBarLayout );

    QAction* newBitmapLayerAct = new QAction( QIcon( ":icons/layer-bitmap.png" ), tr( "New Bitmap Layer" ), this );
    QAction* newVectorLayerAct = new QAction( QIcon( ":icons/layer-vector.png" ), tr( "New Vector Layer" ), this );
    QAction* newSoundLayerAct = new QAction( QIcon( ":icons/layer-sound.png" ), tr( "New Sound Layer" ), this );
    QAction* newCameraLayerAct = new QAction( QIcon( ":icons/layer-camera.png" ), tr( "New Camera Layer" ), this );

    QMenu* layerMenu = new QMenu( tr( "&Layer" ), this );
    layerMenu->addAction( newBitmapLayerAct );
    layerMenu->addAction( newVectorLayerAct );
    layerMenu->addAction( newSoundLayerAct );
    layerMenu->addAction( newCameraLayerAct );
    addLayerButton->setMenu( layerMenu );
    addLayerButton->setPopupMode( QToolButton::InstantPopup );

    QGridLayout* leftLayout = new QGridLayout();
    leftLayout->addWidget( leftToolBar, 0, 0 );
    leftLayout->addWidget( mLayerList, 1, 0 );
    leftLayout->setMargin( 0 );
    leftLayout->setSpacing( 0 );
    leftWidget->setLayout( leftLayout );

    // --- right widget ---
    // --------- key buttons ---------
    QToolBar* keyButtons = new QToolBar( this );
    QLabel* keyLabel = new QLabel( tr( "Keys:" ) );
    keyLabel->setFont( QFont( "Helvetica", 10 ) );
    keyLabel->setIndent( 5 );

    QToolButton* addKeyButton = new QToolButton( this );
    addKeyButton->setIcon( QIcon( ":icons/add.png" ) );
    addKeyButton->setToolTip( "Add Frame" );
    addKeyButton->setFixedSize( 24, 24 );

    QToolButton* removeKeyButton = new QToolButton( this );
    removeKeyButton->setIcon( QIcon( ":icons/remove.png" ) );
    removeKeyButton->setToolTip( "Remove Frame" );
    removeKeyButton->setFixedSize( 24, 24 );

    QToolButton* duplicateKeyButton = new QToolButton( this );
    duplicateKeyButton->setIcon( QIcon( ":icons/controls/duplicate.png" ) );
    duplicateKeyButton->setToolTip( "Duplicate Frame" );
    duplicateKeyButton->setFixedSize( 24, 24 );
    keyButtons->addWidget( keyLabel );
    keyButtons->addWidget( addKeyButton );
    keyButtons->addWidget( removeKeyButton );
    keyButtons->addWidget( duplicateKeyButton );

    // --------- Time controls ---------
    mTimeControls = new TimeControls( this );

    QHBoxLayout* rightToolBarLayout = new QHBoxLayout();
    rightToolBarLayout->addWidget( keyButtons );
    rightToolBarLayout->addStretch( 1 );
    rightToolBarLayout->addWidget( mTimeControls );
    rightToolBarLayout->setMargin( 0 );
    rightToolBarLayout->setSpacing( 0 );
    rightToolBar->setLayout( rightToolBarLayout );

    QGridLayout* rightLayout = new QGridLayout();
    rightLayout->addWidget( rightToolBar, 0, 0 );
    rightLayout->addWidget( mTracks, 1, 0 );
    rightLayout->setMargin( 0 );
    rightLayout->setSpacing( 0 );
    rightWidget->setLayout( rightLayout );

    // --- Splitter ---
    QSplitter* splitter = new QSplitter( this );
    splitter->addWidget(switchWidget);//
    splitter->addWidget( leftWidget );
    splitter->addWidget( rightWidget );
    splitter->setSizes( QList<int>() << 120 <<100 << 600 );


    QGridLayout* lay = new QGridLayout();
    lay->addWidget( splitter, 0, 0 );
    lay->addWidget( vScrollBar, 0, 1 );
    lay->addWidget( hScrollBar, 1, 0 );
    lay->setMargin( 0 );
    lay->setSpacing( 0 );
    timeLineContent->setLayout( lay );
    setWidget( timeLineContent );

    setWindowFlags( Qt::WindowStaysOnTopHint );

    connect( this, &TimeLine::lengthChange, mTracks, &TimeLineCells::lengthChange );
    connect( this, &TimeLine::fontSizeChange, mTracks, &TimeLineCells::fontSizeChange );
    connect( this, &TimeLine::frameSizeChange, mTracks, &TimeLineCells::frameSizeChange );
    connect( this, &TimeLine::labelChange, mTracks, &TimeLineCells::labelChange );
    connect( this, &TimeLine::scrubChange, mTracks, &TimeLineCells::scrubChange );

    connect( hScrollBar, &QScrollBar::valueChanged, mTracks, &TimeLineCells::hScrollChange );
    connect( vScrollBar, &QScrollBar::valueChanged, mTracks, &TimeLineCells::vScrollChange );
    connect( vScrollBar, &QScrollBar::valueChanged, mLayerList, &TimeLineCells::vScrollChange );

    connect(switchButton1,&QToolButton::clicked,this,&TimeLine::openToolBox);
    connect(switchButton2,&QToolButton::clicked,this,&TimeLine::openMaterial);

    connect( addKeyButton,    &QToolButton::clicked, this, &TimeLine::addKeyClick );
    connect( removeKeyButton, &QToolButton::clicked, this, &TimeLine::removeKeyClick );
    connect( duplicateKeyButton, &QToolButton::clicked, this, &TimeLine::duplicateKeyClick );

    connect( mTimeControls, SIGNAL( loopStartClick( int ) ), this, SIGNAL( loopStartClick( int ) ) );
    connect( mTimeControls, SIGNAL( loopEndClick( int ) ), this, SIGNAL( loopEndClick( int ) ) );

    connect( mTimeControls, SIGNAL( soundClick( bool ) ), this, SIGNAL( soundClick( bool ) ) );
    connect( mTimeControls, SIGNAL( fpsClick( int ) ), this, SIGNAL( fpsClick( int ) ) );

    connect( newBitmapLayerAct, &QAction::triggered, this, &TimeLine::newBitmapLayer );
    connect( newVectorLayerAct, &QAction::triggered, this, &TimeLine::newVectorLayer );
    connect( newSoundLayerAct, &QAction::triggered, this, &TimeLine::newSoundLayer );
    connect( newCameraLayerAct, &QAction::triggered, this, &TimeLine::newCameraLayer );
    connect( removeLayerButton, &QPushButton::clicked, this, &TimeLine::deleteCurrentLayer );
    
    mTimeControls->setCore( editor() );

    scrubbing = false;
}

void TimeLine::updateUI()
{
    mTracks->update();
    mLayerList->update();


}

int TimeLine::getFrameLength()
{
    return mTracks->getFrameLength();
}

void TimeLine::resizeEvent(QResizeEvent*)
{
    updateLayerView();
}

void TimeLine::deleteCurrentLayer()
{
    QString strLayerName = editor()->layers()->currentLayer()->name();

    int ret = QMessageBox::warning( this,
                                    tr( "Warning" ),
                                    tr( "Are you sure you want to delete layer: " ) + strLayerName + " ?",
                                    QMessageBox::Ok | QMessageBox::Cancel,
                                    QMessageBox::Ok );
    if ( ret == QMessageBox::Ok )
    {
        editor()->layers()->deleteCurrentLayer();
    }
}

void TimeLine::updateFrame( int frameNumber )
{
    Q_ASSERT ( mTracks );

    mTracks->updateFrame( mLastUpdatedFrame );
    mTracks->updateFrame( frameNumber );

    mLastUpdatedFrame = frameNumber;
}

void TimeLine::updateLayerView()
{
    vScrollBar->setPageStep( (height()-mTracks->getOffsetY()-hScrollBar->height())/mTracks->getLayerHeight() -2 );
    vScrollBar->setMinimum( 0 );
    vScrollBar->setMaximum( qMax(0, mNumLayers - vScrollBar->pageStep()) );
    update();
    updateContent();
}

void TimeLine::updateLayerNumber(int numberOfLayers)
{
    this->mNumLayers = numberOfLayers;
    updateLayerView();
}

void TimeLine::updateLength(int frameLength)
{
    hScrollBar->setMaximum( frameLength );
}

void TimeLine::updateContent()
{
    mLayerList->updateContent();
    mTracks->updateContent();
    update();
}

void TimeLine::setFps ( int value )
{
    mTimeControls->setFps(value);
}

void TimeLine::forceUpdateLength(QString newLength)
{
    bool ok;
    int dec = newLength.toInt(&ok, 10);

    if ( dec > getFrameLength())
    {
        updateLength(dec);
        updateContent();
        QSettings settings("Pencil","Pencil");
        settings.setValue("length", dec);
    }
}
