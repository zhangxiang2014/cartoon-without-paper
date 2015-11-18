#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDockWidget>
namespace Ui {
class Dialog;
}
class QListWidget;
class QGraphicsView;
class QScrollArea;
class QMouseEvent;
class QGraphicsScene;
class QMediaPlayer;
class QMediaPlaylist;

class Dialog : public QDockWidget
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
protected:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void mouseMoveEvent(QMouseEvent*event);
    void mousePressEvent(QMouseEvent*event);
    //bool eventFilter(QObject *target, QEvent *event);


private slots:
    void addMediaMethod();
    void deleteMediaMethod();
    void preButtonMethod();
    void addAudioMethod();
    void produceButtonMethod();
    void deleteVideoMethod();
    void deleteAudioMethod();
    void timeDone();
    void produceMethod();

    void listMediaMouseDown();
    void panelMediaDropDown();
    void listAudioMouseDown();
    void panelAudioDropDown();

    void showThePicture();

    void showThisPicture();
    void videoToPic();
    void playTheMusic();
    void addPicButton();
    void addAudioButton(QString content);

    void loadCurrentFrame(QString projectUrl,QString picUrl);
    void reloadFrame();
    void importMediaMethod();
    void importAudioMethod();
signals:
    void saveCurrentFrameSignal(int id);
    void openThisPicture(QString projectUrl,int flagSave);
    //bool eventFilter(QObject *target, QEvent *event);
public:
    int getWidthOfControls(QListWidget* list);
    QDockWidget* getDockWidget();
private:
    Ui::Dialog *ui;
    QListWidget* listMedia;
    QListWidget* listAudio;
    QDialog* dialog;
    QDockWidget *dockWidget;

    QGraphicsView* pictureBox;
    QGraphicsScene *scene;
    QImage* image;
    QTimer* timerThread;
    QMediaPlayer* player;
    QMediaPlaylist* playlist;
    int currentPos;
    int cursorBeforeClick;
    bool flagMove;
    QStringList* listName;
    QPoint startPoint;
    //QScrollArea* scrollArea;
public:
    QListWidget* listMediaBelow;
    QListWidget* listAudioBelow;

};

#endif // DIALOG_H
