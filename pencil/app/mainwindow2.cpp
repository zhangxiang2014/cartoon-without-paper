/*

Pencil - Traditional Animation Software
Copyright (C) 2005-2007 Patrick Corrieri & Pascal Naidon
Copyright (C) 2008-2009 Mj Mendoza IV

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation;

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/

#include "mainwindow2.h"
#include "ui_mainwindow2.h"

#include <memory>
#include <QList>
#include <QMenu>
#include <QFile>
#include <QScopedPointer>
#include <QMessageBox>
#include <QFileDialog>
#include <QProgressDialog>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <memory>
#include <iostream>
#include <QApplication>
#include <QClipboard>
#include <QBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QSvgGenerator>
#include <QMessageBox>
#include <QImageReader>
#include <QComboBox>
#include <QSlider>
#include <QFileDialog>
#include <QInputDialog>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QDragEnterEvent>
#include <QDropEvent>
#include "pencildef.h"
#include "pencilsettings.h"

#include "object.h"
#include "objectsaveloader.h"

#include "editor.h"
#include "colormanager.h"
#include "layermanager.h"
#include "layerbitmap.h"
#include "toolmanager.h"
#include "playbackmanager.h"

#include "scribblearea.h"
#include "colorpalettewidget.h"
#include "displayoptiondockwidget.h"
#include "tooloptiondockwidget.h"
#include "popupcolorpalettewidget.h"
#include "preferences.h"
#include "timeline.h"
#include "toolbox.h"
#include "preview.h"
#include "timeline2.h"
#include "menutool.h"

#include "colorbox.h"
#include "util.h"

#include "fileformat.h"     //contains constants used by Pencil File Format
#include "JlCompress.h"     //compress and decompress New Pencil File Format
#include "recentfilemenu.h"

#include "exportimageseqdialog.h"


MainWindow2::MainWindow2( QWidget *parent ) : QMainWindow( parent )
{
    setObjectName("MainWindow");
    ui = new Ui::MainWindow2;
    ui->setupUi( this );
   // setWindowFlags(Qt::FramelessWindowHint);

    // Central widget
    mScribbleArea = new ScribbleArea( this );
    mScribbleArea->setObjectName( "ScribbleArea" );
    mScribbleArea->setFocusPolicy( Qt::StrongFocus );
    setCentralWidget( mScribbleArea );//把scribbleArea放到了最中央

    Object* object = new Object();
    object->init();

    mEditor = new Editor( this );
    mEditor->initialize( mScribbleArea );
    mEditor->setObject( object );

    mScribbleArea->setCore( mEditor );
    mEditor->setScribbleArea( mScribbleArea );
    makeConnections( mEditor, mScribbleArea );

    createDockWidgets();
    createMenus();
    setupKeyboardShortcuts();

    mEditor->resetUI();

    readSettings();

    connect( mEditor, &Editor::needSave, this, &MainWindow2::saveDocument );
    connect( mToolBox, &ToolBoxWidget::clearButtonClicked, mEditor, &Editor::clearCurrentFrame );

    //connect( mScribbleArea, &ScribbleArea::refreshPreview, mPreview, &PreviewWidget::updateImage );

    mEditor->setCurrentLayer( mEditor->object()->getLayerCount() - 1 );
    mEditor->tools()->setDefaultTool();

    FrameImageURL = QDir::homePath()+"/CartoonImage"+QString::number(currentImageIndex,10)+".png";//add by xiang
    ProjectURL =QDir::homePath()+"/sunProject"+QString::number(currentImageIndex,10)+ PFF_EXTENSION;
    sunProjectURLToImageURL[ProjectURL] = FrameImageURL;
}



MainWindow2::~MainWindow2()
{
    delete ui;
}

void MainWindow2::createDockWidgets()
{
    //setWindowFlags(Qt::FramelessWindowHint);
    mTimeLine = new TimeLine( this );
    mTimeLine->setObjectName( "TimeLine" );
    mDockWidgets.append( mTimeLine );

    mColorWheel = new QDockWidget( tr("Color Wheel"), this );
    ColorBox* pColorBox = new ColorBox(this);
    pColorBox->setToolTip(tr("color palette:<br>use <b>(C)</b><br>toggle at cursor"));
    mColorWheel->setWidget( pColorBox );
    mColorWheel->setObjectName( "ColorWheel" );
    //mDockWidgets.append( mColorWheel);//我加的
    mColorWheel->NoDockWidgetFeatures;
    mColorWheel->setTitleBarWidget(new QWidget);

    mColorPalette = new ColorPaletteWidget( tr( "Color Palette" ), this );
    mColorPalette->setMaximumWidth(100);
    mColorPalette->setObjectName( "ColorPalette" );
    mColorPalette->setStyleSheet(QString::fromUtf8("background-image: url(:icons/b2.jpg)"));
    mColorPalette->setWindowFlags(Qt::FramelessWindowHint);
    mDockWidgets.append( mColorPalette );

    mDisplayOptionWidget = new DisplayOptionWidget(this);
    mDisplayOptionWidget->setObjectName( "DisplayOption" );

    mToolOptions = new ToolOptionWidget(this);
    mToolOptions->setStyleSheet("background-image: url(:icons/b2.jpg)");
    mToolOptions->setWindowFlags(Qt::FramelessWindowHint);

    mToolOptions->setObjectName( "ToolOption" );

    mToolBox = new ToolBoxWidget( tr( "Tools" ), this );
    mToolBox->setObjectName( "ToolBox" );
//    mToolBox->setAutoFillBackground(true);
//    QPalette palette;
//    palette.setBrush(QPalette::Background, QBrush(QPixmap(":icons/b1.jpg")));
//    mToolBox->setPalette(palette);
   // mToolBox->setStyleSheet(QString::fromUtf8("background-image: url(:icons/b1.jpg)"));

//    mToolBox->setWindowFlags(Qt::WindowMaximizeButtonHint);
//    mToolBox->setWindowFlags(Qt::WindowMinMaxButtonsHint);
//    mToolBox->setWindowFlags(Qt::WindowCloseButtonHint);
    mDockWidgets.append( mToolBox );

//    mMenuTool = new MenuToolWidget( tr( "MenuTools" ), this );
//    mMenuTool->setObjectName("MenuTool");
//    mDockWidgets.append(mMenuTool);

//    mTimeline2 = new Timeline2;
//    mTimeline2->setObjectName( "Timeline2" );
//    mDockWidgets.append( mTimeline2 );

//    mToolBox->setWindowFlags(Qt::FramelessWindowHint);
//    mColorWheel->setWindowFlags(Qt::FramelessWindowHint);
//    mColorPalette->setWindowFlags(Qt::FramelessWindowHint);
//    mDisplayOptionWidget->setWindowFlags(Qt::FramelessWindowHint);
//    mToolOptions->setWindowFlags(Qt::FramelessWindowHint);
    //tabifyDockWidget(mColorWheel,mColorPalette);

//    addDockWidget(Qt::TopDockWidgetArea, mMenuTool);
//    addDockWidget(Qt::RightDockWidgetArea,  mColorWheel);
//    addDockWidget(Qt::RightDockWidgetArea,  mColorPalette);
//   // addDockWidget(Qt::RightDockWidgetArea,  mDisplayOptionWidget);
//    addDockWidget(Qt::LeftDockWidgetArea,   mToolBox);
//    //addDockWidget(Qt::LeftDockWidgetArea,   mToolOptions);
//    addDockWidget(Qt::BottomDockWidgetArea, mTimeLine);
//   // addDockWidget( Qt::BottomDockWidgetArea, mTimeline2);
   // mMenuTool->setFloating(false);
    mColorWheel->setFloating(false);
    mDisplayOptionWidget->setFloating(false);
    mColorPalette->setFloating(false);
    mToolBox->setFloating(false);
    mTimeLine->setFloating(false);
    //mMenuTool->close();
    mColorWheel->close();
    mDisplayOptionWidget->close();
    mColorPalette->close();
    mToolBox->close();
    mTimeLine->close();
    //mTimeline2->setFloating(false);
    for ( BaseDockWidget* pWidget : mDockWidgets )
    {
        pWidget->setCore( mEditor );
        pWidget->initUI();
        pWidget->setFeatures( QDockWidget::AllDockWidgetFeatures );
        pWidget->setFocusPolicy( Qt::NoFocus );
        pWidget->hide();
     //   pWidget->setWindowFlags(Qt::FramelessWindowHint);     //也是去掉标题栏的语句);
        qDebug() << "Init UI: " << pWidget->objectName();
    }

    /*
    mPreview = new PreviewWidget( this );
    mPreview->setImage( mScribbleArea->mBufferImg );
    mPreview->setFeatures( QDockWidget::DockWidgetFloatable );
    mPreview->setFocusPolicy( Qt::NoFocus );
    addDockWidget( Qt::RightDockWidgetArea, mPreview );
    */
   // makeConnections( mMenuTool,mToolBox);
    makeConnections( mEditor, mTimeLine );
    makeConnections( mEditor, pColorBox );
    makeConnections( mEditor, mColorPalette );
    makeConnections( mEditor, mDisplayOptionWidget );
    makeConnections( mEditor, mToolOptions );
    mToolOptions->makeConnectionToEditor(mEditor);
}


void MainWindow2::createMenus()
{
    // ---------- File Menu -------------
  // connect( ui->actionNew, &QAction::triggered, this, &MainWindow2::newDocument );
    connect( ui->actionOpen, &QAction::triggered, this, &MainWindow2::openDocument );
    connect( ui->actionSave_as, &QAction::triggered, this, &MainWindow2::saveAsNewDocument );
    connect( ui->actionSave, &QAction::triggered, this, &MainWindow2::saveDocument );
    connect( ui->actionExit, &QAction::triggered, this, &MainWindow2::close );

    /// --- Export Menu ---
    //connect( ui->actionExport_X_sheet, &QAction::triggered, mEditor, &Editor::exportX );
    connect( ui->actionExport_Image, &QAction::triggered, this, &MainWindow2::exportImage );
    connect( ui->actionExport_Image_Sequence, &QAction::triggered, this, &MainWindow2::exportImageSequence );
    connect( ui->actionExport_Movie, &QAction::triggered, this, &MainWindow2::exportMovie );

    connect( ui->actionExport_Palette, &QAction::triggered, this, &MainWindow2::exportPalette );

    /// --- Import Menu ---
    //connect( ui->actionExport_Svg_Image, &QAction::triggered, editor, &Editor::saveSvg );
    connect( ui->actionImport_Image, &QAction::triggered, this, &MainWindow2::importImage );
    connect( ui->actionImport_Image_Sequence, &QAction::triggered, this, &MainWindow2::importImageSequence );
    connect( ui->actionImport_Movie, &QAction::triggered, this, &MainWindow2::importMovie );

    //connect( ui->actionImport_Sound, &QAction::triggered, editor, &Editor::importSound );
    ui->actionImport_Sound->setEnabled( false );
    connect( ui->actionImport_Palette, &QAction::triggered, this, &MainWindow2::importPalette );

    /// --- Edit Menu ---
    ui->actionPreference->setMenuRole( QAction::PreferencesRole );

    connect( ui->actionUndo, &QAction::triggered, mEditor, &Editor::undo );
    connect( ui->actionRedo, &QAction::triggered, mEditor, &Editor::redo );
    connect( ui->actionCut, &QAction::triggered, mEditor, &Editor::cut );
    connect( ui->actionCopy, &QAction::triggered, mEditor, &Editor::copy );
    connect( ui->actionPaste, &QAction::triggered, mEditor, &Editor::paste );
    connect( ui->actionClearFrame, &QAction::triggered, mEditor, &Editor::clearCurrentFrame );
    connect( ui->actionFlip_X, &QAction::triggered, mEditor, &Editor::flipX );
    connect( ui->actionFlip_Y, &QAction::triggered, mEditor, &Editor::flipY );
    connect( ui->actionSelect_All, &QAction::triggered, mEditor, &Editor::selectAll );
    connect( ui->actionDeselect_All, &QAction::triggered, mEditor, &Editor::deselectAll );
    connect( ui->actionPreference, &QAction::triggered, [=] { preferences(); } );

    ui->actionRedo->setEnabled( false );

    /// --- Layer Menu ---
    connect( ui->actionNew_Bitmap_Layer, &QAction::triggered, mEditor, &Editor::newBitmapLayer );
    connect( ui->actionNew_Vector_Layer, &QAction::triggered, mEditor, &Editor::newVectorLayer );
    connect( ui->actionNew_Sound_Layer, &QAction::triggered, mEditor, &Editor::newSoundLayer );
    connect( ui->actionNew_Camera_Layer, &QAction::triggered, mEditor, &Editor::newCameraLayer );
    connect( ui->actionDelete_Current_Layer, &QAction::triggered, mEditor->layers(), &LayerManager::deleteCurrentLayer );

    /// --- View Menu ---
    connect( ui->actionZoom_In, &QAction::triggered, mEditor, &Editor::zoomIn );
    connect( ui->actionZoom_Out, &QAction::triggered, mEditor, &Editor::zoomOut );
    connect( ui->actionRotate_Clockwise, &QAction::triggered, mEditor, &Editor::rotatecw );
    connect( ui->actionRotate_Anticlosewise, &QAction::triggered, mEditor, &Editor::rotateacw );
    connect( ui->actionReset_Windows, &QAction::triggered, this, &MainWindow2::dockAllPalettes );
    connect( ui->actionReset_View, &QAction::triggered, mEditor, &Editor::resetView );
    connect( ui->actionHorizontal_Flip, &QAction::triggered, mEditor, &Editor::toggleMirror );
    connect( ui->actionVertical_Flip, &QAction::triggered, mEditor, &Editor::toggleMirrorV );

    ui->actionPreview->setEnabled( false );
    //# connect(previewAct, SIGNAL(triggered()), editor, SLOT(getCameraLayer()));//TODO: Preview view

    connect( ui->actionGrid, &QAction::triggered, [ = ]( bool bChecked )
    {
        mScribbleArea->setEffect( EFFECT_GRID_A, bChecked );
    } );



    connect( ui->actionOnionPrevious, &QAction::triggered, mEditor, &Editor::toggleOnionPrev );
    connect( ui->actionOnionNext, &QAction::triggered, mEditor, &Editor::toggleOnionNext );
    connect( ui->actionMultiLayerOnionSkin, &QAction::triggered, mEditor, &Editor::toggleMultiLayerOnionSkin );

    connect( mEditor, &Editor::onionPrevChanged, ui->actionOnionPrevious, &QAction::setChecked );
    connect( mEditor, &Editor::onionNextChanged, ui->actionOnionNext, &QAction::setChecked );
    connect( mEditor, SIGNAL(multiLayerOnionSkinChanged(bool)), ui->actionMultiLayerOnionSkin, SLOT(setChecked(bool)));

    /// --- Animation Menu ---
    PlaybackManager* pPlaybackManager = mEditor->playback();
    connect( ui->actionPlay, &QAction::triggered, pPlaybackManager, &PlaybackManager::play );

    connect( ui->actionLoop, &QAction::triggered, pPlaybackManager, &PlaybackManager::setLooping );
    connect( ui->actionLoopControl, &QAction::triggered, pPlaybackManager, &PlaybackManager::enableRangedPlayback );
    connect( pPlaybackManager, &PlaybackManager::loopStateChanged, ui->actionLoop, &QAction::setChecked );
    connect( pPlaybackManager, &PlaybackManager::rangedPlaybackStateChanged, ui->actionLoopControl, &QAction::setChecked );

    connect(ui->actionAdd_Frame, &QAction::triggered, mEditor, &Editor::addNewKey );
    connect(ui->actionRemove_Frame, &QAction::triggered, mEditor, &Editor::removeKey );
    //connect(ui->actionNext_Frame, &QAction::triggered, m_pEditor, &Editor::playNextFrame );
    //connect(ui->actionPrevious_Frame, &QAction::triggered, m_pEditor, &Editor::playPrevFrame );
    connect(ui->actionNext_KeyFrame, &QAction::triggered, mEditor, &Editor::scrubNextKeyFrame );
    connect(ui->actionPrev_KeyFrame, &QAction::triggered, mEditor, &Editor::scrubPreviousKeyFrame );
    connect(ui->actionDuplicate_Frame, &QAction::triggered, mEditor, &Editor::duplicateKey );
    connect(ui->actionMove_Frame_Forward, &QAction::triggered, mEditor, &Editor::moveFrameForward ); //HERE
    connect(ui->actionMove_Frame_Backward, &QAction::triggered, mEditor, &Editor::moveFrameBackward );

    /// --- Tool Menu ---
    connect(ui->actionMove, &QAction::triggered, mToolBox, &ToolBoxWidget::moveOn );
    connect(ui->actionSelect, &QAction::triggered, mToolBox, &ToolBoxWidget::selectOn );
    connect(ui->actionBrush, &QAction::triggered, mToolBox, &ToolBoxWidget::brushOn );
    connect(ui->actionPolyline, &QAction::triggered, mToolBox, &ToolBoxWidget::polylineOn );
    connect(ui->actionSmudge, &QAction::triggered, mToolBox, &ToolBoxWidget::smudgeOn );
    connect(ui->actionPen, &QAction::triggered, mToolBox, &ToolBoxWidget::penOn );
    connect(ui->actionHand, &QAction::triggered, mToolBox, &ToolBoxWidget::handOn );
    connect(ui->actionPencil, &QAction::triggered, mToolBox, &ToolBoxWidget::pencilOn );
    connect(ui->actionBucket, &QAction::triggered, mToolBox, &ToolBoxWidget::bucketOn );
    connect(ui->actionEyedropper, &QAction::triggered, mToolBox, &ToolBoxWidget::eyedropperOn );
    connect(ui->actionEraser, &QAction::triggered, mToolBox, &ToolBoxWidget::eraserOn );
    connect(ui->actionTogglePalette, &QAction::triggered, mScribbleArea,&ScribbleArea::togglePopupPalette );
    connect(ui->actionResetToolsDefault, &QAction::triggered, mEditor->tools(), &ToolManager::resetAllTools );

    /// --- Window Menu ---
    QMenu* winMenu = ui->menuWindows;

    QAction* actions[] =
    {
        mToolBox->toggleViewAction(),
        mToolOptions->toggleViewAction(),
        mColorWheel->toggleViewAction(),
        mColorPalette->toggleViewAction(),
        mTimeLine->toggleViewAction(),
        mDisplayOptionWidget->toggleViewAction()
    };
    winMenu->clear();
    for ( QAction* action : actions )
    {
        action->setMenuRole( QAction::NoRole );
        winMenu->addAction( action );
    }

    /// --- Help Menu ---
    connect( ui->actionHelp, &QAction::triggered, this, &MainWindow2::helpBox);
    connect( ui->actionAbout, &QAction::triggered, this, &MainWindow2::aboutPencil );

    // --------------- Menus ------------------
    mRecentFileMenu = new RecentFileMenu( tr("Open Recent"), this );
    mRecentFileMenu->loadFromDisk();
    ui->menuFile->insertMenu( ui->actionSave, mRecentFileMenu );

    QObject::connect( mRecentFileMenu, SIGNAL( loadRecentFile( QString ) ),
        this, SLOT( openFile( QString ) ) );

    connect( ui->menuEdit, SIGNAL( aboutToShow() ), this, SLOT( undoActSetText() ) );
    connect( ui->menuEdit, SIGNAL( aboutToHide() ), this, SLOT( undoActSetEnabled() ) );
//    winMenu->menuAction()->setVisible(false);
//    ui->menuView->menuAction()->setVisible(false);
//    ui->menuFile->menuAction()->setVisible(false);
//    ui->menuEdit->menuAction()->setVisible(false);
//    ui->menuAnimation->menuAction()->setVisible(false);
//  //  ui->menuTools->menuAction()->setVisible(false);
//    ui->menuLayer->menuAction()->setVisible(false);
//    ui->menuHelp->menuAction()->setVisible(false);
}

void MainWindow2::setOpacity( int opacity )
{
    QSettings settings( "Pencil", "Pencil" );
    settings.setValue( "windowOpacity", 100 - opacity );
    setWindowOpacity( opacity / 100.0 );
}

void MainWindow2::closeEvent( QCloseEvent* event )
{
    if ( maybeSave() )
    {
        writeSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow2::tabletEvent( QTabletEvent* event )
{
    event->ignore();
}

void MainWindow2::newDocument()
{
    currentImageIndex++;

    FrameImageURL = QDir::homePath()+"/CartoonImage"+QString::number(currentImageIndex,10)+".png";//add by xiang
    ProjectURL =QDir::homePath()+"/sunProject"+QString::number(currentImageIndex,10)+ PFF_EXTENSION;
    sunProjectURLToImageURL[ProjectURL] = FrameImageURL;
    if ( maybeSave() )
    {
        Object* object = new Object();
        object->init();
        mEditor->setObject( object );
        mEditor->resetUI();

        setWindowTitle( PENCIL_WINDOW_TITLE );
    }
}

void MainWindow2::saveProject()
{
//    FrameImageURL = QDir::homePath()+"/CartoonImage"+QString::number(currentImageIndex,10)+".png";//add by xiang
//    ProjectURL =QDir::homePath()+"/sunProject"+QString::number(currentImageIndex,10)+ PFF_EXTENSION;
//    sunProjectURLToImageURL[ProjectURL] = FrameImageURL;
    saveDocument();
    exportImage();
   // QMessageBox::information(this,"saveURL",ProjectURL);
    emit postURL(ProjectURL,FrameImageURL);
    newDocument();
    mEditor->clearCurrentFrame();
}

void MainWindow2::reopenProject(QString changeFrameURL)
{
    saveDocument();
    exportImage();
    //QMessageBox::information(this,"before reopen,saveCurrentURL",ProjectURL);
    emit postURL(ProjectURL,FrameImageURL);
    openURLDocument(changeFrameURL);
    //QMessageBox::information(this,"reopen the URLProject",changeFrameURL);
    ProjectURL = changeFrameURL;
    FrameImageURL =sunProjectURLToImageURL[ProjectURL];
}

void MainWindow2::UIsetColor(QColor rgb)
{
    const QColor &m_rgb =rgb;
    mEditor->color()->setColor(m_rgb);

}

void MainWindow2::openURLDocument(QString URL)
{
    bool ok = openObject(URL );

    if ( !ok )
    {
        QMessageBox::warning( this, tr("Warning"), tr("Pencil cannot read this file. If you want to import images, use the command import.") );
    }
}

void MainWindow2::openDocument()
{
    if ( maybeSave() )
    {
        QSettings settings( "Pencil", "Pencil" );

        QString strLastOpenPath = settings.value( "lastFilePath", QDir::homePath() ).toString();
        QString fileName = QFileDialog::getOpenFileName( this,
                                                         tr( "Open File..." ),
                                                         strLastOpenPath,
                                                         tr( PFF_OPEN_ALL_FILE_FILTER ) );

        if ( fileName.isEmpty() )
        {
            return;
        }

        QFileInfo fileInfo( fileName );
        if ( fileInfo.isDir() )
        {
            return;
        }

        bool ok = openObject( fileName );

        if ( !ok )
        {
            QMessageBox::warning( this, tr("Warning"), tr("Pencil cannot read this file. If you want to import images, use the command import.") );
            newDocument();
        }
    }
}

bool MainWindow2::saveAsNewDocument()
{
    QSettings settings( "Pencil", "Pencil" );

//    QString strLastFolder = settings.value( "lastFilePath", QDir::homePath() ).toString();
//    if ( strLastFolder.isEmpty() )
//    {
//        strLastFolder = QDir( QDir::homePath() ).filePath( PFF_DEFAULT_FILENAME );
//    }

//    QString fileName = QFileDialog::getSaveFileName( this,
//                                                     tr( "Save As..." ),
//                                                     strLastFolder,
//                                                     tr( PFF_SAVE_ALL_FILE_FILTER ) );
//    if ( fileName.isEmpty() )
//    {
//        return false;
//    }

//    if ( !fileName.endsWith( PFF_OLD_EXTENSION ) && !fileName.endsWith( PFF_EXTENSION ) )
//    {
//        fileName = fileName + PFF_EXTENSION;
//    }
    //add by xiang
    //QString fileName =QDir::homePath()+"/sunProject"+QString::number(currentImageIndex,10)+ PFF_EXTENSION;
    //ProjectURL = fileName;
    settings.setValue( "lastFilePath", QVariant( ProjectURL ) );
    return saveObject( ProjectURL );

}

void MainWindow2::importMaterial(QString materialURL)
{
    mEditor->newBitmapLayer();
    QImageReader reader( materialURL );
  //  QMessageBox::information(this,"aaa",materialURL);
    Q_ASSERT( mEditor->layers()->currentLayer()->type() == Layer::BITMAP );
    auto layer = static_cast<LayerBitmap*>(mEditor->layers()->currentLayer() );

    QImage img;//( reader.size(), QImage::Format_ARGB32_Premultiplied );
    img.load(materialURL);
    while ( reader.read( &img ) )
    {/*
        if ( img.isNull() || reader.nextImageDelay() <= 0 )
        {
            qDebug("why not image");
            break;
        }*/

        if ( !layer->keyExists( mEditor->currentFrame() ) )
        {
            mEditor->addNewKey();
        }


        BitmapImage* bitmapImage = layer->getBitmapImageAtFrame( mEditor->currentFrame() );

        QRect boundaries = img.rect();

        boundaries.moveTopLeft(mScribbleArea->getCentralPoint().toPoint() - QPoint( boundaries.width() / 2, boundaries.height() / 2 ) );
        BitmapImage* importedBitmapImage = new BitmapImage( boundaries, img );
        bitmapImage->paste( importedBitmapImage );

      if(mEditor->layers()->currentLayerIndex()>2)
      {
        //下面用来框出倒入的图片
        mScribbleArea->mySelection.moveTopLeft(mScribbleArea->getCentralPoint().toPoint()- QPoint( boundaries.width()/1.5, boundaries.height()/1.5) );//这个是为了将值赋给QRectF
        mScribbleArea->mySelection.moveBottomRight(mScribbleArea->getCentralPoint().toPoint() + QPoint( boundaries.width()/1.5, boundaries.height()/1.5));
        mScribbleArea->mySelection.setTopLeft(mScribbleArea->getCentralPoint().toPoint() - QPoint( boundaries.width()/1.5, boundaries.height()/1.5));
        mScribbleArea->mySelection.setBottomRight(mScribbleArea->getCentralPoint().toPoint() + QPoint( boundaries.width()/1.5, boundaries.height()/1.5));
        //mScribbleArea->mySelection.moveBottomRight(QPoint( boundaries.width()/2, boundaries.height()/2));
        mScribbleArea->setSelection( mScribbleArea->mySelection, true );
        mScribbleArea->myTransformedSelection = mScribbleArea->mySelection.adjusted( 0, 0, 0, 0 );
        mScribbleArea->myTempTransformedSelection = mScribbleArea->mySelection.adjusted( 0, 0, 0, 0 );
        mScribbleArea->update();
        mScribbleArea->mIncludeImg[mEditor->layers()->currentLayerIndex()]=mScribbleArea->mySelection;
        mEditor->tools()->setCurrentTool(MOVE);
       }
       // scrubTo( currentFrame() + 1 );
    }
}

void MainWindow2::whatToolOpen(int toolIndex)
{
    materialFlag = 0;
    switch(toolIndex)
    {
    case 0:
        mToolBox->setCurrentTool(MOVE);
        break;
    case 1:
        mToolBox->setCurrentTool(BUCKET);
        break;
    case 2:
        mToolBox->setCurrentTool(BRUSH);
        break;
    case 3:
        mToolBox->setCurrentTool(ERASER);
        break;
    case 4:
        mToolBox->setCurrentTool(SMUDGE);
        break;
    case 5:
        mToolBox->setCurrentTool(SELECT);
        break;
    case 6:
        mEditor->clearCurrentFrame();//当第12个按钮，清除当前幕布
        break;
    case 7:
        fixtion();
        break;
    case 8:
        mToolBox->setCurrentTool(EYEDROPPER);
        break;
    case 9:
        //mToolBox->setCurrentTool(PEN);
        break;
    case 10:
        //mToolBox->setCurrentTool(PENCIL);
        break;
    case 12:
        //mEditor->clearCurrentFrame();//当第12个按钮，清除当前幕布
        break;

    default:
        break;
    }
}

void MainWindow2::topToolOpen(int topIndex)
{
    switch(topIndex)
    {
    case 0:
       // newDocument();
        break;
    case 1:
        openDocument();
        break;
    case 2:
        break;
    case 3:
        mEditor->cut();
        break;
    case 4:
        mEditor->copy();
        break;
    case 5:
        mEditor->paste();
        break;
    case 6:
        mEditor->undo();
        break;
    case 7:
        mEditor->redo();
        break;
    case 8:
        //洋葱皮功能
        break;
    default:
        break;
    }
}

void MainWindow2::sliderWidthValue(int newWidth)
{
    mEditor->tools()->currentTool()->setWidth((float)newWidth);

}

void MainWindow2::sliderFeatherValue(int newFeather)
{
    mEditor->tools()->currentTool()->setFeather((float)newFeather);
}

void MainWindow2::openFile( QString filename )
{
    qDebug() << "open recent file" << filename;
    bool ok = openObject( filename );
    if ( !ok )
    {
        QMessageBox::warning( this, tr("Warning"), tr("Pencil cannot read this file. If you want to import images, use the command import.") );
        newDocument();
    }
}

bool MainWindow2::openObject( QString strFilePath )
{
    QProgressDialog progress( tr("Opening document..."), tr("Abort"), 0, 100, this );

    progress.setWindowModality( Qt::WindowModal );
    progress.show();

    mEditor->setCurrentLayer( 0 );

    ObjectSaveLoader objectLoader( this );
    Object* object = objectLoader.load( strFilePath );

    if ( object == nullptr || objectLoader.error().code() != PCL_OK )
    {
        return false;
    }

    mEditor->setObject( object );

    QSettings settings( "Pencil", "Pencil" );
    settings.setValue( "LastFilePath", object->filePath() );

    mRecentFileMenu->addRecentFile( object->filePath() );
    mRecentFileMenu->saveToDisk();

    //qDebug() << "Current File Path=" << object->filePath();
    setWindowTitle( object->filePath() );

    progress.setValue( 100 );
    return true;
}

bool MainWindow2::saveObject( QString strSavedFileName )
{
    QProgressDialog progress( tr( "Saving document..." ), tr( "Abort" ), 0, 100, this );
    progress.setWindowModality( Qt::WindowModal );
    progress.show();

    ObjectSaveLoader* saveLoader = new ObjectSaveLoader( this );
    bool ok = saveLoader->save( mEditor->object(), strSavedFileName );

    progress.setValue( 100 );

    QSettings settings( "Pencil", "Pencil" );
    settings.setValue( "LastFilePath", strSavedFileName );

    mRecentFileMenu->addRecentFile( strSavedFileName );
    mRecentFileMenu->saveToDisk();

    mTimeLine->updateContent();

    setWindowTitle( strSavedFileName );

    return true;
}

void MainWindow2::saveDocument()
{
    if ( !mEditor->object()->filePath().isEmpty() )
    {
        saveObject( mEditor->object()->filePath() );
    }
    else
    {
        saveAsNewDocument();
    }
}

bool MainWindow2::maybeSave()
{
    if ( mEditor->object()->isModified() )
    {
        int ret = QMessageBox::warning( this, tr( "Warning" ),
                                        tr( "This animation has been modified.\n Do you want to save your changes?" ),
                                        QMessageBox::Yes | QMessageBox::Default,
                                        QMessageBox::No,
                                        QMessageBox::Cancel | QMessageBox::Escape );
        if ( ret == QMessageBox::Yes )
        {
            saveDocument();
            return true;
        }
        else if ( ret == QMessageBox::Cancel )
        {
            return false;
        }
    }
    return true;
}

void MainWindow2::importImage()
{
    QSettings settings( "Pencil", "Pencil" );
    QString initPath = settings.value( "lastImportPath", QDir::homePath() ).toString();

    QString strFilePath = QFileDialog::getOpenFileName( this,
                                                        tr( "Import image..." ),
                                                        initPath,
                                                        PENCIL_IMAGE_FILTER );
    if ( strFilePath.isEmpty() )
    {
        return;
    }

//    if ( QFile::exists( strFilePath ) )
//    {
//        return;
//    }

    bool ok = mEditor->importImage( strFilePath );
    if ( !ok )
    {
        QMessageBox::warning( this,
                              tr( "Warning" ),
                              tr( "Unable to load bitmap image.<br><b>TIP:</b> Use Bitmap layer to import bitmaps." ),
                              QMessageBox::Ok,
                              QMessageBox::Ok );
        return;
    }

    settings.setValue( "lastImportPath", strFilePath );

    mScribbleArea->updateCurrentFrame();
    mTimeLine->updateContent();
}

void MainWindow2::importImageSequence()
{
    QFileDialog w;
    w.setFileMode( QFileDialog::AnyFile );

    QSettings settings( "Pencil", "Pencil" );
    QString initialPath = settings.value( "lastImportPath", QVariant( QDir::homePath() ) ).toString();
    if ( initialPath.isEmpty() )
    {
        initialPath = QDir::homePath();
    }
    QStringList files = w.getOpenFileNames( this,
                                            "Select one or more files to open",
                                            initialPath,
                                            "Images (*.png *.jpg *.jpeg *.bmp *.xml)" );

    for ( QString strImgFile : files )
    {
        if ( strImgFile.endsWith( ".png" ) ||
             strImgFile.endsWith( ".jpg" ) ||
             strImgFile.endsWith( ".jpeg" ) ||
             strImgFile.endsWith( ".bmp" )||
             strImgFile.endsWith(".xml"))
        {
            mEditor->importImage( strImgFile );
            mEditor->scrubForward();
        }
    }
}

void MainWindow2::importMovie()
{
    QSettings settings( "Pencil", "Pencil" );

    QString initialPath = settings.value( "lastExportPath", QDir::homePath() ).toString();
    QString filePath = QFileDialog::getOpenFileName( this,
                                                     tr( "Import movie" ),
                                                     initialPath,
                                                     PENCIL_MOVIE_EXT );
    if ( filePath.isEmpty() )
    {
        return;
    }
    mEditor->importMovie( filePath, mEditor->playback()->fps() );

    settings.setValue( "lastExportPath", filePath );
}

void MainWindow2::exportMovie()
{

}

void MainWindow2::exportImageSequence()
{
    QSettings settings( PENCIL2D, PENCIL2D );

    QString strInitPath = settings.value( "lastExportPath", QDir::homePath() + "/untitled.png" ).toString();
    QString strFilePath = QFileDialog::getSaveFileName( this,
                                                        tr( "Save Image Sequence" ),
                                                        strInitPath,
                                                        PENCIL_IMAGE_FILTER );
    if ( strFilePath.isEmpty() )
    {
        // TODO:
        return; // false;
    }
    settings.setValue( "lastExportPath", strFilePath );

    auto dialog =  new ExportImageSeqDialog( this );
    OnScopeExit( dialog->deleteLater() );

    dialog->setExportSize( mScribbleArea->getViewRect().toRect().size() );
    dialog->exec();

    if ( dialog->result() == QDialog::Rejected )
    {
        return; // false;
    }

    QSize exportSize = dialog->getExportSize();

    QTransform view = RectMapTransform( mScribbleArea->getViewRect(), QRectF( QPointF( 0, 0 ), exportSize ) );
    view = mScribbleArea->getView() * view;

    QString exportFormat = dialog->getExportFormat();
    int projectLength = mEditor->layers()->projectLength();
    mEditor->object()->exportFrames( 1,
                                     projectLength,
                                     mEditor->layers()->currentLayer(),
                                     exportSize,
                                     strFilePath,
                                     exportFormat.toStdString().c_str(),
                                     -1,
                                     false,
                                     true,
                                     NULL,
                                     0 );
    //return true;
}

void MainWindow2::exportImage()
{
    QSettings settings( "Pencil", "Pencil" );
//    QString initPath = settings.value( "lastExportPath", QDir::homePath() + "/untitled.png" ).toString();

//    QString filePath = QFileDialog::getSaveFileName( this,
//                                                     tr( "Save Image" ),
//                                                     initPath,
//                                                    PENCIL_IMAGE_FILTER );


//    if ( filePath.isEmpty() )
//    {
//        qDebug() << "empty file";
//        return;// false;
//    }
//    QFileInfo info( filePath );
//    if ( info.suffix().isEmpty() )
//    {
//        filePath += ".png"; // add PNG as default if the name has no suffix
//    }
    settings.setValue( "lastExportPath", QVariant(  FrameImageURL ) );

    QSize exportSize = mScribbleArea->getViewRect().toRect().size();
    QTransform view = RectMapTransform( mScribbleArea->getViewRect(), QRectF( QPointF( -550, -300 ), exportSize ) );
    view = mScribbleArea->getView() * view;

    int projectLength = mEditor->layers()->projectLength();
    if ( !mEditor->object()->exportIm( mEditor->currentFrame(), projectLength, view, exportSize, FrameImageURL, true ) )
    {
        QMessageBox::warning( this,
                              tr( "Warning" ),
                              tr( "Unable to export image." ),
                              QMessageBox::Ok,
                              QMessageBox::Ok );
        return;// false;
    }
    return; // true;
}

void MainWindow2::importSound()
{
    Layer* layer = mEditor->layers()->currentLayer();
    if ( layer == NULL )
    {
        QMessageBox msg;
        msg.setText( "You must select an empty sound layer as the destination for your sound before importing. Please create a new sound layer." );
        msg.setIcon( QMessageBox::Warning );
        msg.exec();
        return;
    }

    if ( layer->type() != Layer::SOUND )
    {
        QMessageBox msg;
        msg.setText( "No sound layer exists as a destination for your import. Create a new sound layer?" );
        QAbstractButton* acceptButton = msg.addButton( "Create sound layer", QMessageBox::AcceptRole );
        msg.addButton( "Don't create layer", QMessageBox::RejectRole );

        msg.exec();
        if ( msg.clickedButton() == acceptButton )
        {
            //SoundLayer();
            layer = mEditor->layers()->currentLayer();
        }
        else
        {
            return;
        }
    }
    /*
    if ( !( ( LayerSound* )layer )->isEmpty() )
    {
        QMessageBox msg;
        msg.setText( "The sound layer you have selected already contains a sound item. Please select another." );
        msg.exec();
        return;
    }

    if ( filePath.isEmpty() || filePath == "fromDialog" )
    {
        QSettings settings( "Pencil", "Pencil" );
        QString initialPath = settings.value( "lastImportPath", QVariant( QDir::homePath() ) ).toString();
        if ( initialPath.isEmpty() ) initialPath = QDir::homePath();
        filePath = QFileDialog::getOpenFileName( mMainWindow, tr( "Import sound..." ), initialPath, tr( "WAV(*.wav);;MP3(*.mp3)" ) );
        if ( !filePath.isEmpty() )
        {
            settings.setValue( "lastImportPath", QVariant( filePath ) );
        }
        else
        {
            return;
        }
    }
    ( ( LayerSound* )layer )->loadSoundAtFrame( filePath, currentFrame() );
    */
    mTimeLine->updateContent();
}

void MainWindow2::preferences()
{
    m_pPreferences = new Preferences( this );

    connect( m_pPreferences, SIGNAL( lengthSizeChange( QString ) ), mTimeLine, SIGNAL( lengthChange( QString ) ) );
    connect( m_pPreferences, SIGNAL( fontSizeChange( int ) ), mTimeLine, SIGNAL( fontSizeChange( int ) ) );
    connect( m_pPreferences, SIGNAL( frameSizeChange( int ) ), mTimeLine, SIGNAL( frameSizeChange( int ) ) );
    connect( m_pPreferences, SIGNAL( labelChange( int ) ), mTimeLine, SIGNAL( labelChange( int ) ) );
    connect( m_pPreferences, SIGNAL( scrubChange( int ) ), mTimeLine, SIGNAL( scrubChange( int ) ) );

    connect( m_pPreferences, SIGNAL( windowOpacityChange( int ) ), this, SLOT( setOpacity( int ) ) );
    connect( m_pPreferences, SIGNAL( curveSmoothingChange( int ) ), mScribbleArea, SLOT( setCurveSmoothing( int ) ) );
    connect( m_pPreferences, SIGNAL( antialiasingChange( int ) ), mScribbleArea, SLOT( setAntialiasing( int ) ) );
    connect( m_pPreferences, SIGNAL( backgroundChange( int ) ), mScribbleArea, SLOT( setBackground( int ) ) );
    connect( m_pPreferences, SIGNAL( toolCursorsChange( int ) ),  SLOT( setToolCursors( int ) ) );
    connect( m_pPreferences, SIGNAL( styleChanged( int ) ), mScribbleArea, SLOT( setStyle( int ) ) );

    connect( m_pPreferences, SIGNAL( autosaveChange( int ) ), mEditor, SLOT( changeAutosave( int ) ) );
    connect( m_pPreferences, SIGNAL( autosaveNumberChange( int ) ), mEditor, SLOT( changeAutosaveNumber( int ) ) );
mScribbleArea,
    connect( m_pPreferences, SIGNAL( onionMaxOpacityChange( int ) ), mEditor, SLOT( onionMaxOpacityChangeSlot( int ) ) );
    connect( m_pPreferences, SIGNAL( onionMinOpacityChange( int ) ), mEditor, SLOT( onionMinOpacityChangeSlot( int ) ) );
    connect( m_pPreferences, SIGNAL( onionPrevFramesNumChange( int ) ), mEditor, SLOT( onionPrevFramesNumChangeSlot( int ) ) );
    connect( m_pPreferences, SIGNAL( onionNextFramesNumChange( int ) ), mEditor, SLOT( onionNextFramesNumChangeSlot( int ) ) );

    clearKeyboardShortcuts();
    connect( m_pPreferences, &Preferences::destroyed, [=] { setupKeyboardShortcuts(); } );

    m_pPreferences->show();
}

void MainWindow2::dockAllPalettes()
{
    mToolBox->setFloating(false);
    mToolOptions->setFloating(false);
    mDisplayOptionWidget->setFloating(false);
    mTimeLine->setFloating(false);
    mColorPalette->setFloating(false);
    mColorWheel->setFloating( false );
}

void MainWindow2::readSettings()
{
    qDebug( "Restore last windows layout." );

    QSettings settings( PENCIL2D, PENCIL2D );
    restoreGeometry( settings.value( SETTING_WINDOW_GEOMETRY ).toByteArray() );
    restoreState( settings.value( SETTING_WINDOW_STATE ).toByteArray() );

    QString myPath = settings.value( "lastFilePath", QVariant( QDir::homePath() ) ).toString();
    mRecentFileMenu->addRecentFile( myPath );

    setOpacity( 100 - settings.value( "windowOpacity" ).toInt() );
}

void MainWindow2::writeSettings()
{
    qDebug( "Save current windows layout." );

    QSettings settings( PENCIL2D, PENCIL2D );
    settings.setValue( SETTING_WINDOW_GEOMETRY, saveGeometry() );
    settings.setValue( SETTING_WINDOW_STATE, saveState() );
}

void MainWindow2::setupKeyboardShortcuts()
{
    checkExistingShortcuts();

    auto cmdKeySeq = []( QString strCommandName ) -> QKeySequence
    {
        strCommandName = QString( "shortcuts/" ) + strCommandName;
        QKeySequence keySequence( pencilSettings()->value( strCommandName ).toString() );
        return keySequence;
    };

    ui->actionNew->setShortcut( cmdKeySeq( CMD_NEW_FILE ) );
    ui->actionOpen->setShortcut( cmdKeySeq( CMD_OPEN_FILE ) );
    ui->actionSave->setShortcut( cmdKeySeq( CMD_SAVE_FILE ) );
    ui->actionSave_as->setShortcut( cmdKeySeq( CMD_SAVE_AS ) );
    ui->actionPrint->setShortcut( cmdKeySeq( CMD_PRINT ) );

    ui->actionImport_Image->setShortcut( cmdKeySeq( CMD_IMPORT_IMAGE ) );
    ui->actionImport_Image_Sequence->setShortcut( cmdKeySeq( CMD_IMPORT_IMAGE_SEQ ) );
    ui->actionImport_Movie->setShortcut( cmdKeySeq( CMD_IMPORT_MOVIE ) );
    ui->actionImport_Palette->setShortcut( cmdKeySeq( CMD_IMPORT_PALETTE ) );
    ui->actionImport_Sound->setShortcut( cmdKeySeq( CMD_IMPORT_SOUND ) );

    ui->actionExport_Image->setShortcut( cmdKeySeq( CMD_EXPORT_IMAGE ) );
    ui->actionExport_Image_Sequence->setShortcut( cmdKeySeq( CMD_EXPORT_IMAGE_SEQ ) );
    ui->actionExport_Movie->setShortcut( cmdKeySeq( CMD_EXPORT_MOVIE ) );
    ui->actionExport_Palette->setShortcut( cmdKeySeq( CMD_EXPORT_PALETTE ) );
    ui->actionExport_Svg_Image->setShortcut( cmdKeySeq( CMD_EXPORT_SVG ) );
    ui->actionExport_X_sheet->setShortcut( cmdKeySeq( CMD_EXPORT_XSHEET ) );

    // edit menu
    ui->actionUndo->setShortcut( cmdKeySeq( CMD_UNDO ) );
    ui->actionRedo->setShortcut( cmdKeySeq( CMD_REDO ) );
    ui->actionCut->setShortcut( cmdKeySeq( CMD_CUT ) );
    ui->actionCopy->setShortcut( cmdKeySeq( CMD_COPY ) );
    ui->actionPaste->setShortcut( cmdKeySeq( CMD_PASTE ) );
    ui->actionClearFrame->setShortcut( cmdKeySeq( CMD_CLEAR_FRAME ) );
    ui->actionSelect_All->setShortcut( cmdKeySeq( CMD_SELECT_ALL ) );
    ui->actionDeselect_All->setShortcut( cmdKeySeq( CMD_DESELECT_ALL ) );
    ui->actionPreference->setShortcut( cmdKeySeq( CMD_PREFERENCE ) );

    ui->actionReset_Windows->setShortcut( cmdKeySeq( CMD_RESET_WINDOWS ) );
    ui->actionReset_View->setShortcut( cmdKeySeq( CMD_RESET_ZOOM_ROTATE ) );
    ui->actionZoom_In->setShortcut( cmdKeySeq( CMD_ZOOM_IN ) );
    ui->actionZoom_Out->setShortcut( cmdKeySeq( CMD_ZOOM_OUT ) );
    ui->actionRotate_Clockwise->setShortcut( cmdKeySeq( CMD_ROTATE_CLOCK ) );
    ui->actionRotate_Anticlosewise->setShortcut( cmdKeySeq( CMD_ROTATE_ANTI_CLOCK ) );
    ui->actionHorizontal_Flip->setShortcut( cmdKeySeq( CMD_FLIP_HORIZONTAL ) );
    ui->actionVertical_Flip->setShortcut( cmdKeySeq( CMD_FLIP_VERTICAL ) );
    ui->actionPreview->setShortcut( cmdKeySeq( CMD_PREVIEW ) );
    ui->actionGrid->setShortcut( cmdKeySeq( CMD_GRID ) );
    ui->actionOnionPrevious->setShortcut( cmdKeySeq( CMD_ONIONSKIN_PREV ) );
    ui->actionOnionNext->setShortcut( cmdKeySeq( CMD_ONIONSKIN_NEXT ) );

    ui->actionPlay->setShortcut( cmdKeySeq( CMD_PLAY ) );
    ui->actionLoop->setShortcut( cmdKeySeq( CMD_LOOP ) );
    ui->actionPrevious_Frame->setShortcut( cmdKeySeq( CMD_GOTO_PREV_FRAME ) );
    ui->actionNext_Frame->setShortcut( cmdKeySeq( CMD_GOTO_NEXT_FRAME ) );
    ui->actionPrev_KeyFrame->setShortcut( cmdKeySeq( CMD_GOTO_PREV_KEY_FRAME ) );
    ui->actionNext_KeyFrame->setShortcut( cmdKeySeq( CMD_GOTO_NEXT_KEY_FRAME ) );
    ui->actionAdd_Frame->setShortcut( cmdKeySeq( CMD_ADD_FRAME ) );
    ui->actionDuplicate_Frame->setShortcut( cmdKeySeq( CMD_DUPLICATE_FRAME ) );
    ui->actionRemove_Frame->setShortcut( cmdKeySeq( CMD_REMOVE_FRAME ) );
    ui->actionMove_Frame_Backward->setShortcut( cmdKeySeq( CMD_MOVE_FRAME_BACKWARD ) );
    ui->actionMove_Frame_Forward->setShortcut( cmdKeySeq( CMD_MOVE_FRAME_FORWARD ) );

    ui->actionMove->setShortcut( cmdKeySeq( CMD_TOOL_MOVE ) );
    ui->actionSelect->setShortcut( cmdKeySeq( CMD_TOOL_SELECT ) );
    ui->actionBrush->setShortcut( cmdKeySeq( CMD_TOOL_BRUSH ) );
    ui->actionPolyline->setShortcut( cmdKeySeq( CMD_TOOL_POLYLINE ) );
    ui->actionSmudge->setShortcut( cmdKeySeq( CMD_TOOL_SMUDGE ) );
    ui->actionPen->setShortcut( cmdKeySeq( CMD_TOOL_PEN ) );
    ui->actionHand->setShortcut( cmdKeySeq( CMD_TOOL_HAND ) );
    ui->actionPencil->setShortcut( cmdKeySeq( CMD_TOOL_PENCIL ) );
    ui->actionBucket->setShortcut( cmdKeySeq( CMD_TOOL_BUCKET ) );
    ui->actionEyedropper->setShortcut( cmdKeySeq( CMD_TOOL_EYEDROPPER ) );
    ui->actionEraser->setShortcut( cmdKeySeq( CMD_TOOL_ERASER ) );

    ui->actionTogglePalette->setShortcut( cmdKeySeq( CMD_TOGGLE_PALETTE ) );
    mScribbleArea->getPopupPalette()->closeButton->setText( tr("close/toggle (") + pencilSettings()->value( QString( "shortcuts/" ) + CMD_TOGGLE_PALETTE ).toString() + ")" );
    mScribbleArea->getPopupPalette()->closeButton->setShortcut( cmdKeySeq( CMD_TOGGLE_PALETTE ) );

    ui->actionNew_Bitmap_Layer->setShortcut( cmdKeySeq( CMD_NEW_BITMAP_LAYER ) );
    ui->actionNew_Vector_Layer->setShortcut( cmdKeySeq( CMD_NEW_VECTOR_LAYER ) );
    ui->actionNew_Camera_Layer->setShortcut( cmdKeySeq( CMD_NEW_CAMERA_LAYER ) );
    ui->actionNew_Sound_Layer->setShortcut( cmdKeySeq( CMD_NEW_SOUND_LAYER ) );

    mToolBox->toggleViewAction()->setShortcut( cmdKeySeq( CMD_TOGGLE_TOOLBOX ) );
    mToolOptions->toggleViewAction()->setShortcut( cmdKeySeq( CMD_TOGGLE_TOOL_OPTIONS ) );
    mColorWheel->toggleViewAction()->setShortcut( cmdKeySeq( CMD_TOGGLE_COLOR_WHEEL ) );
    mColorPalette->toggleViewAction()->setShortcut( cmdKeySeq( CMD_TOGGLE_COLOR_LIBRARY ) );
    mTimeLine->toggleViewAction()->setShortcut( cmdKeySeq( CMD_TOGGLE_TIMELINE ) );
    mDisplayOptionWidget->toggleViewAction()->setShortcut( cmdKeySeq( CMD_TOGGLE_DISPLAY_OPTIONS ) );

    ui->actionHelp->setShortcut( cmdKeySeq( CMD_HELP ) );
}

void MainWindow2::clearKeyboardShortcuts()
{
    QList<QAction*> actionList = this->findChildren<QAction*>();
    foreach( QAction* action, actionList )
    {
        action->setShortcut( QKeySequence( 0 ) );
    }
}

void MainWindow2::undoActSetText( void )
{
    if ( this->mEditor->mBackupIndex < 0 )
    {
        ui->actionUndo->setText( tr("Undo") );
        ui->actionUndo->setEnabled( false );
    }
    else
    {
        ui->actionUndo->setText( tr("Undo   ")
                                + QString::number( this->mEditor->mBackupIndex + 1 )
                                + " "
                                + this->mEditor->mBackupList.at( this->mEditor->mBackupIndex )->undoText );
        ui->actionUndo->setEnabled( true );
    }

    if ( this->mEditor->mBackupIndex + 2 < this->mEditor->mBackupList.size() )
    {
        ui->actionRedo->setText( tr("Redo   ")
                                + QString::number( this->mEditor->mBackupIndex + 2 )
                                + " "
                                + this->mEditor->mBackupList.at( this->mEditor->mBackupIndex + 1 )->undoText );
        ui->actionRedo->setEnabled( true );
    }
    else
    {
        ui->actionRedo->setText( tr("Redo") );
        ui->actionRedo->setEnabled( false );
    }
}

void MainWindow2::undoActSetEnabled( void )
{
    ui->actionUndo->setEnabled( true );
    ui->actionRedo->setEnabled( true );
}

void MainWindow2::exportPalette()
{
    QSettings settings( "Pencil", "Pencil" );
    QString initialPath = settings.value( "lastPalettePath", QVariant( QDir::homePath() ) ).toString();
    if ( initialPath.isEmpty() )
    {
        initialPath = QDir::homePath() + "/untitled.xml";
    }
    QString filePath = QFileDialog::getSaveFileName( this, tr( "Export As" ), initialPath );
    if ( !filePath.isEmpty() )
    {
        mEditor->object()->exportPalette( filePath );
        settings.setValue( "lastPalettePath", QVariant( filePath ) );
    }
}

void MainWindow2::importPalette()
{
    QSettings settings( "Pencil", "Pencil" );
    QString initialPath = settings.value( "lastPalettePath", QVariant( QDir::homePath() ) ).toString();
    if ( initialPath.isEmpty() )
    {
        initialPath = QDir::homePath() + "/untitled.xml";
    }
    QString filePath = QFileDialog::getOpenFileName( this, tr( "Import" ), initialPath );
    if ( !filePath.isEmpty() )
    {
        mEditor->object()->importPalette( filePath );
        mColorPalette->refreshColorList();
        settings.setValue( "lastPalettePath", QVariant( filePath ) );
    }
}

void MainWindow2::aboutPencil()
{
    QFile aboutFile( ":resources/about.html" );
    bool isOpenOK = aboutFile.open( QIODevice::ReadOnly | QIODevice::Text );

    if ( isOpenOK )
    {
        QString strAboutText = QTextStream( &aboutFile ).readAll();
        QMessageBox::about( this, PENCIL_WINDOW_TITLE, strAboutText );
    }
}

void MainWindow2::helpBox()
{
    qDebug() << "Open help manual.";

    QString url = "http://www.pencil2d.org/documentation/";
    QDesktopServices::openUrl( QUrl(url) );
}

void MainWindow2::controlColorWheel()
{
    if(controlFlag=="true")
    {
        mColorWheel->show();
        controlFlag ="false";
        mColorPalette->close();
    }
    else{
        mColorWheel->close();
        controlFlag = "true";
        mColorPalette->show();
    }

}

//void MainWindow2::makeConnections(MenuToolWidget* menuTool,ToolBoxWidget* toolBox)
//{
//    connect(menuTool->openButton,&QToolButton::clicked, this, &MainWindow2::openDocument );
//    connect(menuTool->saveButton,&QToolButton::clicked, this, &MainWindow2::saveDocument );
//    connect(menuTool->loadButton,&QToolButton::clicked, this ,&MainWindow2::exportImage );
//    connect(menuTool->cutButton,&QToolButton::clicked,mEditor,&Editor::cut);
//    connect(menuTool->copyButton,&QToolButton::clicked,mEditor,&Editor::copy);
//    connect(menuTool->pasteButton,&QToolButton::clicked,mEditor,&Editor::paste);
//    connect(menuTool->recycleButton,&QToolButton::clicked,mEditor,&Editor::redo);
//    connect(menuTool->undoButton,&QToolButton::clicked,mEditor,&Editor::undo);
//    connect(menuTool->onionupButton,&QToolButton::clicked,mEditor,&Editor::onionPrevChanged);
//    connect(menuTool->oniondownButton,&QToolButton::clicked,mEditor,&Editor::onionNextChanged);
//    connect(toolBox->handButton,&QToolButton::clicked,this,&MainWindow2::controlColorWheel);
//}
void MainWindow2::makeConnections( Editor* editor, ColorBox* colorBox )
{
    connect( colorBox, &ColorBox::colorChanged, editor->color(), &ColorManager::setColor );
    connect( editor->color(), &ColorManager::colorChanged, colorBox, &ColorBox::setColor );
}

void MainWindow2::makeConnections( Editor* editor, ScribbleArea* scribbleArea )
{
    connect( editor->tools(), &ToolManager::toolChanged, scribbleArea, &ScribbleArea::setCurrentTool );
    connect( editor->tools(), &ToolManager::toolPropertyChanged, scribbleArea, &ScribbleArea::updateToolCursor );

    connect( editor, &Editor::currentFrameChanged, scribbleArea, &ScribbleArea::updateFrame );
    connect( editor, &Editor::updateAllFrames, scribbleArea, &ScribbleArea::updateAllFrames );

    connect( editor, &Editor::toggleOnionPrev, scribbleArea, &ScribbleArea::toggleOnionPrev );
    connect( editor, &Editor::toggleOnionNext, scribbleArea, &ScribbleArea::toggleOnionNext );
    connect( editor, &Editor::toggleMultiLayerOnionSkin, scribbleArea, &ScribbleArea::toggleMultiLayerOnionSkin );

    connect( scribbleArea, &ScribbleArea::thinLinesChanged, editor, &Editor::changeThinLinesButton );
    connect( scribbleArea, &ScribbleArea::outlinesChanged, editor, &Editor::changeOutlinesButton );
    connect( scribbleArea, &ScribbleArea::onionPrevChanged, editor, &Editor::onionPrevChanged );
    connect( scribbleArea, &ScribbleArea::onionNextChanged, editor, &Editor::onionNextChanged );

    connect( editor, &Editor::selectAll, scribbleArea, &ScribbleArea::selectAll );
}

void MainWindow2::makeConnections( Editor* pEditor, TimeLine* pTimeline )
{
    PlaybackManager* pPlaybackManager = pEditor->playback();
    //LayerManager* pLayerManager = pEditor->layers();

    connect( pTimeline, &TimeLine::duplicateKeyClick, pEditor, &Editor::duplicateKey );

    connect( pTimeline, &TimeLine::loopStartClick, pPlaybackManager, &PlaybackManager::setRangedStartFrame );
    connect( pTimeline, &TimeLine::loopEndClick, pPlaybackManager, &PlaybackManager::setRangedEndFrame );

    connect( pTimeline, &TimeLine::soundClick, pPlaybackManager, &PlaybackManager::enbaleSound );
    connect( pTimeline, &TimeLine::fpsClick, pPlaybackManager, &PlaybackManager::setFps );


    connect( pTimeline, &TimeLine::addKeyClick, pEditor, &Editor::addNewKey );
    connect( pTimeline, &TimeLine::removeKeyClick, pEditor, &Editor::removeKey );

    connect( pTimeline, &TimeLine::newBitmapLayer, pEditor, &Editor::newBitmapLayer );
    connect( pTimeline, &TimeLine::newVectorLayer, pEditor, &Editor::newVectorLayer );
    connect( pTimeline, &TimeLine::newSoundLayer, pEditor, &Editor::newSoundLayer );
    connect( pTimeline, &TimeLine::newCameraLayer, pEditor, &Editor::newCameraLayer );

    connect( pEditor->layers(), &LayerManager::currentLayerChanged, pTimeline, &TimeLine::updateUI );
    //打开关闭toolBox
  //  connect(pTimeline,&TimeLine::openToolBox,this,&MainWindow2::open_ToolBox);
    connect(pTimeline,&TimeLine::openMaterial,this,&MainWindow2::fixtion);
    connect(pTimeline,&TimeLine::openToolBox,this,&MainWindow2::open_ToolBox);
}

//void MainWindow2::open_ToolBox()
//{
//     mToolBox->close();

//}

void MainWindow2::fixtion()
{
              Layer* layer = mEditor->layers()->currentLayer();
              g_selectCutBitmapImage =((LayerBitmap*)layer)->getLastBitmapImageAtFrame(mEditor->currentFrame(),0)->copy(mScribbleArea->mIncludeImg[mEditor->layers()->currentLayerIndex()].toRect());
              if(mEditor->layers()->currentLayerIndex()>2)
              {
//                  int indexNow = mEditor->layers()->currentLayerIndex();
//                  //mousePress
//                  mScribbleArea->paintTransformedSelection();
//                  mScribbleArea->deselectAll();
//                  mEditor->setCurrentLayer(2);
//                  //mouseRelease
//                  mScribbleArea->getOffset().setX( 0 );
//                  mScribbleArea->getOffset().setY( 0 );
//                  mScribbleArea->calculateSelectionTransformation();
//                  mScribbleArea->myTransformedSelection = mScribbleArea->myTempTransformedSelection;
//      //            if(mEditor->layers()->currentLayerIndex()>2)
//      //                mScribbleArea->mIncludeImg[mEditor->layers()->currentLayerIndex()]=mScribbleArea->myTempTransformedSelection;
//                  mScribbleArea->setModified( mEditor->layers()->currentLayerIndex(), mEditor->currentFrame() );
//                  mScribbleArea->setAllDirty();

//                  //mousePress in selectArea
//                  mEditor->setCurrentLayer(indexNow);
//                  mScribbleArea->mySelection=mScribbleArea->mIncludeImg[mEditor->layers()->currentLayerIndex()];//+
//                   mScribbleArea->myTransformedSelection = mScribbleArea->mySelection.adjusted( 0, 0, 0, 0 );//+
//                   mScribbleArea->myTempTransformedSelection = mScribbleArea->mySelection.adjusted( 0, 0, 0, 0 );//+
//                  mScribbleArea->setSelection( mScribbleArea->mIncludeImg[mEditor->layers()->currentLayerIndex()], true );
//                  mScribbleArea->mIncludeImg[mEditor->layers()->currentLayerIndex()]=mScribbleArea->mySelection;
//                  mScribbleArea->paintTransformedSelection();
//                  mScribbleArea->deselectAll();

//                  //mouseRelease
//                  mScribbleArea->getOffset().setX( 0 );
//                  mScribbleArea->getOffset().setY( 0 );
//                  mScribbleArea->calculateSelectionTransformation();
//                  mScribbleArea->myTransformedSelection = mScribbleArea->myTempTransformedSelection;
//      //            if(mEditor->layers()->currentLayerIndex()>2)
//      //                mScribbleArea->mIncludeImg[mEditor->layers()->currentLayerIndex()]=mScribbleArea->myTempTransformedSelection;
//                  mScribbleArea->setModified( mEditor->layers()->currentLayerIndex(), mEditor->currentFrame() );
//                  mScribbleArea->setAllDirty();


                  mEditor->layers()->deleteCurrentLayer();
                  mEditor->setCurrentLayer(2);
                  Layer* drawLayer = mEditor->layers()->currentLayer();
                  BitmapImage tobePasted =g_selectCutBitmapImage.copy();
                  qDebug() << "to be pasted --->" << tobePasted.image()->size();
                  if ( mScribbleArea->somethingSelected )
                  {
                     QRectF selection = mScribbleArea->getSelection();
                     if ( g_selectCutBitmapImage.width() <= selection.width() && g_selectCutBitmapImage.height() <= selection.height() )
                     {
                           tobePasted.moveTopLeft( selection.topLeft() );
                     }
                     else
                     {
                          tobePasted.transform( selection, true );
                     }
                  }
               auto pLayerBitmap = static_cast< LayerBitmap* >( drawLayer );
               pLayerBitmap->getLastBitmapImageAtFrame( mEditor->currentFrame(), 0 )->paste( &tobePasted ); // paste the clipboard
               mScribbleArea->setSelection(mScribbleArea->getSelection(),false);


            }
    mToolBox->close();
    mToolOptions->close();
    mColorWheel->close();
    mColorPalette->close();
}
//void MoveTool::mouseDoubleClickEvent(QMouseEvent *event)
//{
//  if(mScribbleArea->mIncludeImg[mEditor->layers()->currentLayerIndex()].contains(getCurrentPoint()))
//  {
//      //mEditor->setCurrentLayer(2);
//  }else
//  {
//    for(int i=mEditor->layers()->count();i>=2;i--)
//    {

//        if(mScribbleArea->mIncludeImg[i].contains(getCurrentPoint()))
//        {
// //            mScribbleArea->mySelection=mScribbleArea->mIncludeImg[mEditor->layers()->currentLayerIndex()];//+
// //            mScribbleArea->myTransformedSelection = mScribbleArea->mySelection.adjusted( 0, 0, 0, 0 );//+
// //            mScribbleArea->myTempTransformedSelection = mScribbleArea->mySelection.adjusted( 0, 0, 0, 0 );//+
// //            mScribbleArea->setSelection( mScribbleArea->mIncludeImg[mEditor->layers()->currentLayerIndex()], true );
//            mEditor->setCurrentLayer(i);
//            Layer* layer = mEditor->layers()->currentLayer();
//            g_selectCutBitmapImage =((LayerBitmap*)layer)->getLastBitmapImageAtFrame(mEditor->currentFrame(),0)->copy(mScribbleArea->mIncludeImg[mEditor->layers()->currentLayerIndex()].toRect());
// //            mEditor->layers()->deleteCurrentLayer();

// //            if(i>3&&i<mEditor->layers()->count()){

// //                for(int j=i;j<mEditor->layers()->count();j++)
// //                mScribbleArea->mIncludeImg[j]=mScribbleArea->mIncludeImg[j+1];
// //            }

//            mEditor->setCurrentLayer(2);

//            Layer* drawLayer = mEditor->layers()->currentLayer();
//            BitmapImage tobePasted =g_selectCutBitmapImage.copy();
//            qDebug() << "to be pasted --->" << tobePasted.image()->size();
//            if ( mScribbleArea->somethingSelected )
//            {
//                  QRectF selection = mScribbleArea->getSelection();
//                  if ( g_selectCutBitmapImage.width() <= selection.width() && g_selectCutBitmapImage.height() <= selection.height() )
//                  {
//                        tobePasted.moveTopLeft( selection.topLeft() );
//                  }
//                  else
//                  {
//                        tobePasted.transform( selection, true );
//                  }
//             }
//             auto pLayerBitmap = static_cast< LayerBitmap* >( drawLayer );
//             pLayerBitmap->getLastBitmapImageAtFrame( mEditor->currentFrame(), 0 )->paste( &tobePasted ); // paste the clipboard
//             mScribbleArea->setSelection(mScribbleArea->getSelection(),false);
//             i=1;
//        }
//    }
//  }
// //    Layer *layer = mEditor->layers()->currentLayer();
// //    if ( layer == NULL ) { return; }

// //    if ( event->button() == Qt::LeftButton )
// //    {

// //    }
//}
void MainWindow2::open_ToolBox()
{
    mToolBox->show();
    mToolOptions->show();
//    mToolOptions = new ToolOptionWidget(this);
//    mToolOptions->setStyleSheet("background-image: url(:icons/b2.jpg)");
//    mToolOptions->setWindowFlags(Qt::FramelessWindowHint);

//    mToolOptions->setObjectName( "ToolOption" );

//    mToolBox = new ToolBoxWidget( tr( "Tools" ), this );
//    mToolBox->setObjectName( "ToolBox" );
//    mToolBox->setWindowFlags(Qt::FramelessWindowHint);
//    mToolBox->setWindowFlags(Qt::WindowMaximizeButtonHint);
//    mToolBox->setWindowFlags(Qt::WindowMinMaxButtonsHint);
//    mToolBox->setWindowFlags(Qt::WindowCloseButtonHint);
//    mDockWidgets.append( mToolBox );

//    addDockWidget(Qt::LeftDockWidgetArea,   mToolBox);
//    addDockWidget(Qt::LeftDockWidgetArea,   mToolOptions);
}

void MainWindow2::makeConnections(Editor* editor, DisplayOptionWidget* display)
{
    display->makeConnectionToEditor( editor );
}

void MainWindow2::makeConnections(Editor* editor, ToolOptionWidget* toolOptions)
{
    connect( editor->tools(), &ToolManager::displayToolOptions, toolOptions, &ToolOptionWidget::displayToolOptions );
}


void MainWindow2::makeConnections( Editor* pEditor, ColorPaletteWidget* pColorPalette )
{
    connect( pEditor, &Editor::fileLoaded, pColorPalette, &ColorPaletteWidget::updateUI );

    ColorManager* pColorManager = pEditor->color();
    connect( pColorPalette, &ColorPaletteWidget::colorChanged, pColorManager, &ColorManager::setColor );
    connect( pColorPalette, &ColorPaletteWidget::colorNumberChanged, pColorManager, &ColorManager::setColorNumber );

    connect( pColorManager, &ColorManager::colorChanged, pColorPalette, &ColorPaletteWidget::setColor );
    connect( pColorManager, &ColorManager::colorNumberChanged, pColorPalette, &ColorPaletteWidget::selectColorNumber );
}
