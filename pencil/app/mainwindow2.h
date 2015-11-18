#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include <QDomElement>
#include <QMainWindow>
#include "menutool.h"
#include <QString>
#include <bitmapimage.h>
#include <QMap>
template<typename T> class QList;
class QActionGroup;
class Object;
class Editor;
class ScribbleArea;
class BaseDockWidget;
class ColorPaletteWidget;
class DisplayOptionWidget;
class ToolOptionWidget;
class TimeLine;
class ToolBoxWidget;
class Preferences;
class PreviewWidget;
class ColorBox;
class RecentFileMenu;
class Timeline2;
class MenuToolWidget;


namespace Ui
{
class MainWindow2;
}


static BitmapImage g_selectCutBitmapImage;
class MainWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow2(QWidget* parent = 0);
    ~MainWindow2();

    QString ProjectURL;
    QString FrameImageURL;
    Editor* mEditor;
    QDockWidget*          mColorWheel = nullptr;
    ScribbleArea* mScribbleArea;
    ColorPaletteWidget*   mColorPalette = nullptr;
    DisplayOptionWidget*  mDisplayOptionWidget = nullptr;
    ToolOptionWidget*     mToolOptions = nullptr;
    ToolBoxWidget*        mToolBox;
    MenuToolWidget*       mMenuTool = nullptr;

    //PreviewWidget*           mPreview = nullptr;
    Timeline2*            mTimeline2 = nullptr;
    RecentFileMenu* mRecentFileMenu;
    TimeLine*             mTimeLine; // be public temporary
    long currentImageIndex = 0;
    int materialFlag = 0;
    QMap<QString,QString> sunProjectURLToImageURL;

public slots:
    void setOpacity(int opacity);
    void undoActSetText(void);
    void undoActSetEnabled(void);
    void newDocument();
    void openDocument();
    void openURLDocument(QString URL);//add by xiang
    void saveDocument();
    bool saveAsNewDocument();
    bool maybeSave();
//    void ToolManager::setWidth( float newWidth )
//    {
//        currentTool()->setWidth(newWidth);
//        emit penWidthValueChange( newWidth );
//        emit toolPropertyChanged();
//    }

//    void ToolManager::setFeather( float newFeather )
//    {
//        currentTool()->setFeather(newFeather);
//        emit penFeatherValueChange( newFeather );
//        emit toolPropertyChanged();
//    }

    //interface with cartoon
    void saveProject();
    void reopenProject(QString changeFrameURL);
    //interface with UI
    void whatToolOpen(int toolIndex);
    void topToolOpen(int topIndex);
    void importMaterial(QString materialURL);
    void sliderWidthValue(int newWidth);
    void sliderFeatherValue(int newFeather);
    void UIsetColor(QColor rgb);
    //void colorChange();
//参考下面的代码
//    void MainWindow2::makeConnections( Editor* editor, ColorBox* colorBox )
//    {
//        connect( colorBox, &ColorBox::colorChanged, editor->color(), &ColorManager::setColor );
//        connect( editor->color(), &ColorManager::colorChanged, colorBox, &ColorBox::setColor );
//    }

    // import/export
    void importImage();
    void exportImage();

    void importImageSequence();
    void exportImageSequence();

    void importMovie();
    void exportMovie();

    void importSound();

    void preferences();
    void helpBox();
    void aboutPencil();

    void openFile(QString filename);

    void controlColorWheel();//控制色轮的开关

Q_SIGNALS:
    void postURL(QString Project_URL,QString Image_URL);
    //void isOK();
protected:
    void tabletEvent( QTabletEvent* ) override;
    void closeEvent( QCloseEvent* ) override;

private:
    bool openObject( QString strFilename );
    bool saveObject( QString strFileName );

    void dockAllPalettes();

    void createDockWidgets();
    void createMenus();
    void setupKeyboardShortcuts();
    void clearKeyboardShortcuts();

    void importPalette();
    void exportPalette();
    //打开素材库和工具箱

    void fixtion();
    void open_ToolBox();

    void readSettings();
    void writeSettings();

    void makeConnections( Editor*, ColorBox* );
    void makeConnections( Editor*, ScribbleArea* );
    void makeConnections( Editor*, ColorPaletteWidget* );
    void makeConnections( Editor*, TimeLine* );
    void makeConnections( Editor*, DisplayOptionWidget* );
    void makeConnections( Editor*, ToolOptionWidget*);
    //void makeConnections( MenuToolWidget*,ToolBoxWidget*);

    // UI: central Drawing Area
    //ScribbleArea* mScribbleArea;

    // UI: Dock widgets


    QString controlFlag = "open";//控制色轮的开关

private slots:
    //void on_actionOptionsWidget_triggered();


private:
    Preferences* m_pPreferences;

    Ui::MainWindow2* ui;
    QList< BaseDockWidget* > mDockWidgets;
};

#endif // MAINWINDOW2_H
