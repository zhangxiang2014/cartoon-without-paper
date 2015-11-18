#include "dialog.h"
#include "ui_dialog.h"
#include "qpushbutton.h"
#include "qfiledialog.h"
#include "qmessagebox.h"
#include "qlistwidget.h"
#include "qtimer.h"
#include "qgraphicsscene.h"
#include "qgraphicsview.h"
#include "qframe.h"
#include "qdrag.h"
#include "QDropEvent"
#include "qmimedata.h"
#include "qprocess.h"
#include "qdir.h"
#include "QFileInfo"
#include "QFileInfoList"
#include "qapplication.h"
#include "qtextstream.h"
#include "qevent.h"
#include "qrect.h"
#include "qfileinfo.h"
#include "qscrollarea.h"
#include "QMouseEvent"
#include "qcursor.h"
#include "qscrollbar.h"
#include "QtMultimedia/QMediaPlaylist"
#include "QtMultimedia/QMediaPlayer"
#include "QTime"
#include "QVBoxLayout"

Dialog::Dialog(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi((QDialog*)this);

    //connect buttons to actions
//    QPushButton* addMedia = ui->addMediaButton;
//    connect(addMedia,SIGNAL(clicked()),this,SLOT(addMediaMethod()));
//    addMedia->setVisible(false);

//    QPushButton* deleteMedia = ui->deleteButton;
//    connect(deleteMedia,SIGNAL(clicked()),this,SLOT(deleteMediaMethod()));
//    deleteMedia->setVisible(false);

    QPushButton* preButton = ui->preButton;
    connect(preButton,SIGNAL(clicked()),this,SLOT(preButtonMethod()));

//    QPushButton* addAudio = ui->addAudioButton;
//    connect(addAudio,SIGNAL(clicked()),this,SLOT(addAudioMethod()));
//    addAudio->setVisible(false);

    QPushButton* produceButton = ui->produceButton;
    connect(produceButton,SIGNAL(clicked()),this,SLOT(produceButtonMethod()));

    QPushButton* deleteVideo = ui->videoDeleteButton;
    connect(deleteVideo,SIGNAL(clicked()),this,SLOT(deleteVideoMethod()));

    QPushButton* deleteAudio = ui->audioDeleteButton;
    connect(deleteAudio,SIGNAL(clicked()),this,SLOT(deleteAudioMethod()));

    //QPushButton* addAudioButton = ui->addAudioButton;
    //connect(addAudioButton,SIGNAL(clicked(bool)),this,SLOT(addAudioButton()));

    QPushButton* addPicButton = ui->addPicButton;
    connect(addPicButton,SIGNAL(clicked(bool)),this,SLOT(addPicButton()));

    QPushButton* importMedia = ui->importMediaButton;
    connect(importMedia,SIGNAL(clicked(bool)),this,SLOT(importMediaMethod()));

    QPushButton* importAudio = ui->importAudioButton;
    connect(importAudio,SIGNAL(clicked(bool)),this,SLOT(importAudioMethod()));

    //get the listView
//    listMedia = ui->listWidgetMedia;
//    listMedia->setVisible(false);

//    listAudio = ui->listWidgetAudio;
//    listAudio->setVisible(false);

    pictureBox = new QGraphicsView;//ui->graphicsView;

    listMediaBelow = ui->listMediaWidget;

    listAudioBelow = ui->listAudioWidget;
    //add the event filter ,first register
    //listMediaBelow->installEventFilter(this);
    //listAudioBelow->installEventFilter(this);
    //ui->scrollAreaWidgetContents->installEventFilter(this);
    listMediaBelow->resize(listMediaBelow->width()*15,listMediaBelow->height());
    listAudioBelow->resize(listAudioBelow->width()*15,listAudioBelow->height());
    ui->scrollAreaWidgetContents->setFixedWidth(listMediaBelow->width());
    ui->scrollArea->setWidget(ui->scrollAreaWidgetContents);
    ui->scrollArea->horizontalScrollBar()->setRange(0,listAudioBelow->width());

    //ui->scrollArea->horizontalScrollBar()->setValue(listAudioBelow->width()/2);
    currentPos = 0;
    cursorBeforeClick = 0;
    flagMove = false;
    //configuration of media and audio
    listMediaBelow->setIconSize(QSize(60,60));
    listMediaBelow->setSpacing(3);
    //listAudioBelow->setIconSize(QSize(60,60));
    //listAudioBelow->;
    listAudioBelow->setSpacing(3);
    //connect the event to mousedown
    //connect(listMedia,SIGNAL(itemPressed(QListWidgetItem*)),this,SLOT(listMediaMouseDown()));
    //connect(listAudio,SIGNAL(itemPressed(QListWidgetItem*)),this,SLOT(listAudioMouseDown()));
    //connect(listMedia,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(showThePicture()));
    connect(listMediaBelow,SIGNAL(clicked(QModelIndex)),this,SLOT(showThisPicture()));
    //connect(ui->scrollAreaWidgetContents,SIGNAL(),this,SLOT());
    //connect the pannel to drop
    //connect(listMediaBelow,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(panelMediaDropDown()));

    QListWidgetItem* temp;
    temp = new QListWidgetItem(tr(""));
    temp->setSizeHint(QSize(60,30));
    //temp->setData(2,content);
    temp->setBackgroundColor(QPalette::Base);
    listMediaBelow->addItem(temp);
    listMediaBelow->setCurrentRow(0);
    //ui->scrollAreaWidgetContents->setAcceptDrops(true);
    //listMediaBelow->setAcceptDrops(false);
    //listAudioBelow->setAcceptDrops(false);
    //pictureBox->setAcceptDrops(false);
    setAcceptDrops(true);
    //
    //listName->clear();
    listName = new QStringList;
    player = new QMediaPlayer;
    playlist = new QMediaPlaylist;
    image = new QImage();
    scene = new QGraphicsScene;
    playlist->setPlaybackMode(QMediaPlaylist::Sequential);
    player->setPlaylist(playlist);
    player->setVolume(100);
    connect(preButton,SIGNAL(clicked()),this,SLOT(playTheMusic()));
    //
    QVBoxLayout *box = new QVBoxLayout;
    box->addWidget(pictureBox);
    dialog = new QDialog;
    dialog->setLayout(box);
    dialog->resize(QSize(800,600));
//    QTextStream coutA(stdout, QIODevice::WriteOnly);
//    QString runPath = QCoreApplication::applicationDirPath();
//    coutA << "the runPath is "<< runPath << endl;
//    QDir dir;
//    QString path=dir.currentPath();
//    coutA << "the path is "<< path << endl;
//    QString apppath = QApplication::applicationFilePath();
//    coutA << "the apppath is "<< apppath << endl;
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::dropEvent(QDropEvent *event)
{
    QTextStream coutA(stdout, QIODevice::WriteOnly);
    coutA << "the source is "<< event->source() << endl;
    coutA << "the listMedia is "<< listMedia << endl;
    coutA << "the listAudio is "<< listAudio << endl;
    QRect rect = QRect(ui->scrollArea->pos()+ui->scrollAreaWidgetContents->pos()+ui->listMediaWidget->pos(),ui->listMediaWidget->size());
    QRect rect1 = QRect(ui->scrollArea->pos()+ui->scrollAreaWidgetContents->pos()+ui->listAudioWidget->pos(),ui->listAudioWidget->size());
    if(rect.contains(event->pos()))
    {
       if(listMedia->currentRow()<0||event->source()!=listMedia)
       {
           return;
       }
       QString content = listMedia->item(listMedia->currentRow())->text();
       QString filetype = content.section(".",-1);
       filetype = filetype.toLower();
       int length = 60;
       QPixmap objPixmap;
       QListWidgetItem* temp;
       if(filetype == "avi" || filetype == "wmv" || filetype == "mpg" || filetype == "mp4")
       {
           //get the length
           //get the picture to show in the button
           QString picPath = QCoreApplication::applicationDirPath() + "/videoTemp.jpg";
           QString argments = "ffmpeg -i  " + content + "   -y -f  image2  -ss 1 -vframes 1   " + picPath;
           //QProcess::execute(argments);
           QProcess *pro = new QProcess;
           pro->start(argments);
           pro->waitForFinished();
           QString output = pro->readAllStandardError().data();
           QString theResult = output.mid(output.indexOf("Duration:"),18);
           QStringList list = theResult.split(":");
           QString ltemp = list.at(1);
           QString mtemp = list.at(2);
           QString rtemp = list.at(3);
           int total = ltemp.trimmed().toInt()*3600+mtemp.trimmed().toInt()*60+rtemp.trimmed().toInt();
           if(total==0)
           {
               total=1;
           }
           length = total*length;
           //coutA << "the theResult is "<< QString::number(total) << endl;
           //
           coutA << "the media length data is "<< output << endl;
           objPixmap = QPixmap(picPath);
           temp = new QListWidgetItem((objPixmap.scaled(QSize(60,60),Qt::KeepAspectRatioByExpanding)),tr(""));
           QTimer* timer = new QTimer(this);
           timer->singleShot(10,this,SLOT(videoToPic()));
       }
       else
       {
          objPixmap = QPixmap(content);
          temp = new QListWidgetItem((objPixmap.scaled(QSize(60,60),Qt::KeepAspectRatioByExpanding)),tr(""));
       }
       //test weather to resize the mediaBelow
       if(length+getWidthOfControls(listMediaBelow)>listMediaBelow->width()/2)
       {
           listMediaBelow->resize((length+getWidthOfControls(listMediaBelow))*2,listMediaBelow->height());
           listAudioBelow->resize((length+getWidthOfControls(listMediaBelow))*2,listAudioBelow->height());
           ui->scrollAreaWidgetContents->setFixedWidth(listMediaBelow->width());
       }
       temp->setSizeHint(QSize(length,30));
       temp->setData(2,content);
       coutA << "the data is "<< (temp->data(2)).toString() << endl;
       temp->setBackgroundColor(QPalette::Base);

       listMediaBelow->addItem(temp);
    }else if(rect1.contains(event->pos()))
    {
        if(listAudio->currentRow()<0||event->source()!=listAudio)
        {
            return;
        }
        QString content = listAudio->item(listAudio->currentRow())->text();
        coutA << "the AUDIO is "<< content << endl;
        //QMessageBox::information(NULL, tr("Drop"), content);
        //QPixmap objPixmap(content);
        QListWidgetItem* temp = new QListWidgetItem(content);
        //get length
        QString argmentsAudio = "ffmpeg -i  " + content;
        QProcess *pro = new QProcess;
        pro->start(argmentsAudio);
        pro->waitForFinished();
        QString output = pro->readAllStandardError().data();
        QString theResult = output.mid(output.indexOf("Duration:"),18);
        QStringList list = theResult.split(":");
        QString ltemp = list.at(1);
        QString mtemp = list.at(2);
        QString rtemp = list.at(3);
        int total = ltemp.trimmed().toInt()*3600+mtemp.trimmed().toInt()*60+rtemp.trimmed().toInt();
        if(total==0)
        {
            total=1;
        }
        int lengthAudio = total*60;
        coutA << "the theResult is "<< QString::number(lengthAudio) << endl;
        coutA << "the listAudioBelow->width is "<< QString::number(listAudioBelow->width()) << endl;
        //test weather to resize the audioBelow
        if(lengthAudio+getWidthOfControls(listAudioBelow)>listAudioBelow->width()/2)
        {
            listAudioBelow->resize((lengthAudio+getWidthOfControls(listAudioBelow))*2,listAudioBelow->height());
            listMediaBelow->resize((lengthAudio+getWidthOfControls(listAudioBelow))*2,listMediaBelow->height());
            ui->scrollAreaWidgetContents->setFixedWidth(listMediaBelow->width());
        }
        //
        temp->setSizeHint(QSize(lengthAudio,30));
        temp->setBackgroundColor(QPalette::Base);
        temp->setData(2,content);
        //coutA << "the data is "<< (temp->data(1)).toString() << endl;
        listAudioBelow->addItem(temp);
    }
}

void Dialog::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void Dialog::mouseMoveEvent(QMouseEvent *event)
{
    if(!flagMove)
        return;
    int move = startPoint.x()-event->pos().x();
    if(ui->scrollArea->horizontalScrollBar()->value()+move>ui->scrollArea->horizontalScrollBar()->minimum()&&ui->scrollArea->horizontalScrollBar()->value()+move<ui->scrollArea->horizontalScrollBar()->maximum())
    {
        ui->scrollArea->horizontalScrollBar()->setValue(ui->scrollArea->horizontalScrollBar()->value()+move);
    }
//    QMessageBox::information(NULL, tr("press"), "press left button");
//    if(event->button()==Qt::LeftButton)
//    {
//        QMessageBox::information(NULL, tr("press"), "press left button");
//        int move = event->pos().x() - startPoint.x();
//        if(ui->scrollArea->horizontalScrollBar()->value()+move>ui->scrollArea->horizontalScrollBar()->minimum()&&ui->scrollArea->horizontalScrollBar()->value()+move<ui->scrollArea->horizontalScrollBar()->maximum())
//        {
//            ui->scrollArea->horizontalScrollBar()->setValue(ui->scrollArea->horizontalScrollBar()->value()+move);
//        }
//    }
}

void Dialog::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        startPoint = event->pos();
        QRect rect = QRect(ui->scrollArea->pos()+ui->scrollAreaWidgetContents->pos(),ui->scrollAreaWidgetContents->size());
        if(rect.contains(startPoint))
        {
            flagMove = true;
        }
        else
        {
            flagMove = false;
        }
        //QMessageBox::information(NULL, tr("press"), "press left button");
    }
    else
    {
        event->accept();
    }
}


//bool Dialog::eventFilter(QObject *target, QEvent *event)
//{
//    if(event->type()==QEvent::MouseButtonPress)
//    {
//        QMessageBox::information(NULL, tr("press"), "press left button");
//        return true;
//    }
//    else if(event->type()==QEvent::MouseMove)
//    {
//        return true;
//    }
//    return QDialog::eventFilter(target,event);
//}


void Dialog::addMediaMethod()
{
    QStringList paths = QFileDialog::getOpenFileNames(this, tr("Open Media"), ".", tr("Image Files(*.jpg *.png *.bmp);;Video Files(*.avi *.wmv *.mpg *.mp4)"));
    if(paths.count() == 0) {
        QMessageBox::information(NULL, tr("Open Media"), tr("You didn't select any files."));
    }else{
        listMedia->addItems(paths);
    }
}

void Dialog::deleteMediaMethod()
{
    if(listMedia->currentRow()>=0)
    {
        listMedia->takeItem(listMedia->currentRow());
    }
}

void Dialog::preButtonMethod()
{

    dialog->show();
    //
    listName->clear();
    QTextStream cout(stdout, QIODevice::WriteOnly);
    //pre deal the things
    for(int i=0;i<listMediaBelow->count();i++)
    {
        QString fileName = listMediaBelow->item(i)->data(2).toString();
        if(fileName==NULL)
        {
            continue;
        }
        QString filetype = fileName.section(".",-1);
        filetype = filetype.toLower();
        if(filetype == "avi" || filetype == "wmv" || filetype == "mpg" || filetype == "mp4")
        {
            //QDir dir;dir.currentPath()
            QString str = QCoreApplication::applicationDirPath() + "/"+fileName.section("/",-1);
            QDir temp(str);
            QFileInfoList fileInfoList = temp.entryInfoList();// temp->entryInfoList();
            cout << "the length is "<< fileInfoList.size() << endl;
            cout << "the filename is "<< fileName << endl;
            for(int k=1;k<=fileInfoList.size()-2;k++)
            {
                listName->append(str+"/"+QString::number(k) +".jpg");
                cout << "the file is "<< str+"/"+QString::number(k) +".jpg" << endl;
            }
        }
        else
        {
            listName->append(fileName);
            cout << "the file is "<< fileName << endl;
        }
    }
    currentPos = 0;
    //timer to see the picture demo
    timerThread = new QTimer(this);
    connect(timerThread,SIGNAL(timeout()),this,SLOT(timeDone()));
    timerThread->start(30);
}

void Dialog::addAudioMethod()
{
    QStringList paths = QFileDialog::getOpenFileNames(this, tr("Open Audio"), ".", tr("Audio Files(*.mp3)"));
    if(paths.count() == 0) {
        QMessageBox::information(NULL, tr("Open Audio"), tr("You didn't select any files."));
    }else{
        listAudio->addItems(paths);
    }
}

void Dialog::produceButtonMethod()
{
    //produce
    QTimer* timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->singleShot(50,this,SLOT(produceMethod()));
    //new thread
}

void Dialog::deleteVideoMethod()
{
    if(listMediaBelow->currentRow()<0)
        return;
    listMediaBelow->takeItem(listMediaBelow->currentRow());
}

void Dialog::deleteAudioMethod()
{
    if(listAudioBelow->currentRow()<0)
        return;
    listAudioBelow->takeItem(listAudioBelow->currentRow());
}

void Dialog::timeDone()
{
    QTextStream coutA(stdout, QIODevice::WriteOnly);
    //coutA<<QString::number(currentPos)<<endl;
    //show the picture
    if(currentPos<listName->size())
    {
        int start = currentPos%2==0?QTime::currentTime().msecsSinceStartOfDay():0;
        int end = currentPos%2==1?QTime::currentTime().msecsSinceStartOfDay():0;
        if (start!=0)
            coutA<<QString::number(start)<<endl;
        else
            coutA<<QString::number(end)<<endl;
        QString fileName = listName->at(currentPos);
        if(fileName!=NULL&&image->load(fileName))
        {
            scene->clear();
            scene->addPixmap((QPixmap::fromImage(*image)).scaled(QSize(pictureBox->width(),pictureBox->height()),Qt::KeepAspectRatio));
            pictureBox->setScene(scene);
            //pictureBox->show();
            //pictureBox->repaint();
        }

    }
    else
    {
        if(timerThread->isActive())
        {
            timerThread->stop();
        }
    }
    currentPos++;
}

void Dialog::produceMethod()
{
    QTextStream cout(stdout, QIODevice::WriteOnly);
    //把目录里所有的照片都拷贝到一个临时的目录中，并排序
    //创建相应的文件目录
    QString str = QCoreApplication::applicationDirPath() + "/TeMpPiCsOrT";
    cout << "the path is "<< str << endl;
    QDir * temp = new QDir;
    temp->setPath(str);
    bool exist = temp->exists();

    cout << "the count0 is "<< temp->absoluteFilePath(str) << endl;
    cout << "the count1 is "<< QString::number(temp->Size) << endl;
    temp->setFilter(QDir::NoDotAndDotDot);
    cout << "the count2 is "<< QString::number(temp->count()) << endl;
    cout << "the exit is "<< exist << endl;
    if(exist)
    {
        //delete all
        temp->removeRecursively();
    }
//    else
//    {

//        temp->setFilter(QDir::Files);
//        cout << "the count is "<< QString::number(temp->count()) << endl;
//        for(int a=0;a<temp->count();a++)
//        {
//            cout << "the path is "<< temp[a].absolutePath() << endl;
//            temp->remove(temp[a].absolutePath());
//        }
//    }
    temp->mkdir(str);
    //copy the image to the dir
    //copy the media pictures to the dir
    int m=1;
    for(int i=0;i<listMediaBelow->count();i++)
    {
        QString strTemp = (listMediaBelow->item(i))->data(2).toString();
        if(strTemp==NULL)
        {
            continue;
        }
        QString filetype = strTemp.section(".",-1);
        filetype = filetype.toLower();
        if(filetype == "avi" || filetype == "wmv" || filetype == "mpg" || filetype == "mp4")
        {
            QString source = QCoreApplication::applicationDirPath() + "/"+strTemp.section("/",-1);
            QDir temp(source);
            QFileInfoList fileInfoList = temp.entryInfoList();// temp->entryInfoList();
            cout << "the file length is "<< fileInfoList.length() << endl;
            for(int k=1;k<=fileInfoList.length()-2;k++)
            {
                QFile::copy(source+"/"+QString::number(k) +".png",str+"/"+QString::number(m)+".png");
                m++;
            }
        }
        else
        {
            QPixmap pix;
            pix.load(strTemp);
            pix.save(str+"/"+QString::number(m)+".png","PNG");
            //QFile::copy(strTemp,str+"/"+QString::number(m)+".jpg");
            m++;
        }
        //listName->append(strTemp);
    }
    //QStringList paths = QFileDialog::getOpenFileNames(this, tr("Open Audio"), ".", tr("Audio Files(*.mp3)"));
    //use the ffmpeg
    //通过shell指令生成视频，保存到指定文件夹中
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save Name"),"",tr("AVI files(*.avi)"));
    if(fileName.isNull())
    {
        QMessageBox::information(NULL, tr("No selected"), tr("No input name."));
        return;
    }
    QString picTempPath = str +"/%d.png";
    QString tempPath = QCoreApplication::applicationDirPath()+"/"+fileName.section("/",-1);
    bool existP = temp->exists(tempPath);
    if(existP)
    {
        //delete all
        temp->removeRecursively();
    }
    temp->mkdir(tempPath);
    QString argments = "ffmpeg  -r 24  -i " + picTempPath + " -vcodec mpeg4 -b 2400k  " + tempPath+"/"+fileName.section("/",-1);
    cout << "the command is "<< argments << endl;
    QProcess::execute(argments);
    //add the audio
    QString audioPath = QCoreApplication::applicationDirPath() +"/temp.mp3";
    if(listAudioBelow->count()>0)
    {
        QString strAudioTemp = listAudioBelow->item(0)->data(2).toString();
        QString argmentsAudio = "copy  "+strAudioTemp+"  "+audioPath;
        cout << "the path is "<< argmentsAudio << endl;
        QString argmentsAudio1 = argmentsAudio.replace(QRegExp("/"), "\\");
        system(argmentsAudio1.toLatin1().data());
        if(listAudioBelow->count()>1)
        {

            for(int k=1;k<listAudioBelow->count();k++)
            {
                //tempCmd.clear();
                QString argmentsAudioMore = audioPath + "+" + listAudioBelow->item(k)->data(2).toString() + " " + audioPath;
                QString argmentsAudioMore1 = "copy /b  " + argmentsAudioMore.replace(QRegExp("/"), "\\");
                cout << "the path1 is "<< argmentsAudioMore1 << endl;
                system(argmentsAudioMore1.toLatin1().data());
            }
        }
    }
    else
    {
        QString argmentsMediaCopy = "copy  "+tempPath+"/"+fileName.section("/",-1)+"  "+fileName;
        cout << "the path1 is "<< argmentsMediaCopy << endl;
        QString argmentsMediaCopy1 = argmentsMediaCopy.replace(QRegExp("/"), "\\");
        system(argmentsMediaCopy1.toLatin1().data());
        QMessageBox::information(NULL, tr("Produce "), tr("Produce Succeed."));
        return;
    }
    //mix the audio and video
    QString arArguments = "ffmpeg  -i  " +  tempPath+"/"+fileName.section("/",-1) + "  -i  " + audioPath + "  -vcodec copy -acodec copy  " + fileName;
    QProcess::execute(arArguments);
    cout << "the path is "<< arArguments << endl;
    QMessageBox::information(NULL, tr("Produce "), tr("Produce Succeed."));
}

void Dialog::listMediaMouseDown()
{

}

void Dialog::panelMediaDropDown()
{
    //listMediaBelow->clear();
    QMessageBox::information(NULL, tr("Enter"), tr("Enter the media."));
}

void Dialog::listAudioMouseDown()
{

}

void Dialog::panelAudioDropDown()
{

}

void Dialog::showThePicture()
{
    //show the picture
    QString fileName = listMedia->item(listMedia->currentRow())->text();
    if(fileName==NULL)
    {
        return;
    }
    QImage* image = new QImage();
    if(image->load(fileName))
    {
        QGraphicsScene *scene = new QGraphicsScene;
        scene->addPixmap((QPixmap::fromImage(*image)).scaled(QSize(pictureBox->width(),pictureBox->height()),Qt::KeepAspectRatio));
        pictureBox->setScene(scene);
        pictureBox->show();
        //currentPos++;
    }
}

void Dialog::showThisPicture()
{
    QString fileName = listMediaBelow->item(listMediaBelow->currentRow())->data(2).toString();
    QString projectUrl = listMediaBelow->item(listMediaBelow->currentRow())->data(3).toString();
    if(fileName==NULL)
    {
        return;
    }
//    QImage* image = new QImage;
//    if(image->load(fileName))
//    {
//        QGraphicsScene *scene = new QGraphicsScene;
//        scene->addPixmap((QPixmap::fromImage(*image)).scaled(QSize(pictureBox->width(),pictureBox->height()),Qt::KeepAspectRatio));
//        pictureBox->setScene(scene);
//        pictureBox->show();
//    }
    //reSave the picture
    QListWidgetItem* temp = listMediaBelow->item(listMediaBelow->currentRow());
    //temp->setData(Qt::DisplayRole);
    QIcon icon(fileName);
    temp->setIcon(icon);
    if(cursorBeforeClick==listMediaBelow->count()-1)
    {
        emit openThisPicture(projectUrl,1);
    }
    else
    {
        emit openThisPicture(projectUrl,0);
    }
}

void Dialog::videoToPic()
{
    if(listMedia->currentRow()<0)
        return;
    QString file = listMedia->item(listMedia->currentRow())->text();
    //QDir dir;dir.currentPath()
    QString str = QCoreApplication::applicationDirPath() + "/"+file.section("/",-1);
    QDir * temp = new QDir;
    bool exist = temp->exists(str);
    if(exist)
    {
        //delete all
        temp->removeRecursively();
    }
    temp->mkdir(str);
    QString argments = "ffmpeg   -i  " + file + "   " + str+"/%d.png";
    //cout << "the command is "<< argments << endl;
    QProcess *pro = new QProcess;
    pro->start(argments);
    //pro->waitForFinished();
    //QProcess::execute(argments);
}

void Dialog::playTheMusic()
{
    playlist->clear();
    for(int k=0;k<listAudioBelow->count();k++)
    {
        //tempCmd.clear();
        QString audioPath = listAudioBelow->item(k)->data(2).toString();
        playlist->addMedia(QUrl::fromLocalFile(audioPath));
    }
    if(!playlist->isEmpty())
    {
        if(player->state()==QMediaPlayer::PlayingState)
        {
            player->stop();
        }
        player->play();
    }
}

void Dialog::addPicButton()
{
    //edit the cursor
    //cursorBeforeClick = listMediaBelow->count()-1;
    //add the blank frame
    //focus on it
    QListWidgetItem* temp;
    temp = new QListWidgetItem(tr(""));
    temp->setSizeHint(QSize(60,30));
    //temp->setData(2,content);
    temp->setBackgroundColor(QPalette::Base);
    listMediaBelow->addItem(temp);
    listMediaBelow->setCurrentRow(listMediaBelow->count()-1);
    //send message to save current frame
    emit saveCurrentFrameSignal(listMediaBelow->count()-2);
}

void Dialog::addAudioButton(QString content)
{
    QListWidgetItem* temp = new QListWidgetItem(content);
    //get length
    QString argmentsAudio = "ffmpeg -i  " + content;
    QProcess *pro = new QProcess;
    pro->start(argmentsAudio);
    pro->waitForFinished();
    QString output = pro->readAllStandardError().data();
    QString theResult = output.mid(output.indexOf("Duration:"),18);
    QStringList list = theResult.split(":");
    QString ltemp = list.at(1);
    QString mtemp = list.at(2);
    QString rtemp = list.at(3);
    int total = ltemp.trimmed().toInt()*3600+mtemp.trimmed().toInt()*60+rtemp.trimmed().toInt();
    if(total==0)
    {
        total=1;
    }
    int lengthAudio = total*60;
    //test weather to resize the audioBelow
    if(lengthAudio+getWidthOfControls(listAudioBelow)>listAudioBelow->width()/2)
    {
        listAudioBelow->resize((lengthAudio+getWidthOfControls(listAudioBelow))*2,listAudioBelow->height());
        listMediaBelow->resize((lengthAudio+getWidthOfControls(listAudioBelow))*2,listMediaBelow->height());
        ui->scrollAreaWidgetContents->setFixedWidth(listMediaBelow->width());
    }
    //
    temp->setSizeHint(QSize(lengthAudio,30));
    temp->setBackgroundColor(QPalette::Base);
    temp->setData(2,content);
    //coutA << "the data is "<< (temp->data(1)).toString() << endl;
    listAudioBelow->addItem(temp);
}

void Dialog::loadCurrentFrame(QString projectUrl,QString picUrl)
{
    //load current picture
    //show the picture in the button ,and set the data
    if(listMediaBelow->currentRow()<0)
        return;
    QListWidgetItem* temp = listMediaBelow->item(cursorBeforeClick);
    //QPixmap objPixmap;
    //objPixmap = QPixmap(str);
    //temp = new QListWidgetItem((objPixmap.scaled(QSize(60,60),Qt::KeepAspectRatioByExpanding)),tr(""));
    temp->setData(3,projectUrl);
    temp->setData(2,picUrl);
    //listName->append(picUrl);
    QIcon icon(picUrl);
    temp->setIcon(icon);
    cursorBeforeClick = listMediaBelow->currentRow();
}

void Dialog::reloadFrame()
{
    //reload the project
    QListWidgetItem* temp = listMediaBelow->item(cursorBeforeClick);
    QString str = temp->data(2).toString();
    QIcon icon(str);
    temp->setIcon(icon);
    cursorBeforeClick = listMediaBelow->currentRow();
}

void Dialog::importMediaMethod()
{
    QString content = QFileDialog::getOpenFileName(this, tr("Open Media"), ".", tr("Image Files(*.jpg *.png *.bmp);;Video Files(*.avi *.wmv *.mpg *.mp4)"));
    if(content.isEmpty()||content.isNull())
    {
        QMessageBox::information(NULL, tr("Open Media"), tr("You didn't select any files."));
        return;
    }
    //
    QString filetype = content.section(".",-1);
    filetype = filetype.toLower();
    int length = 60;
    QPixmap objPixmap;
    QListWidgetItem* temp;
    if(filetype == "avi" || filetype == "wmv" || filetype == "mpg" || filetype == "mp4")
    {
        //get the length
        //get the picture to show in the button
        QString picPath = QCoreApplication::applicationDirPath() + "/videoTemp.jpg";
        QString argments = "ffmpeg -i  " + content + "   -y -f  image2  -ss 1 -vframes 1   " + picPath;
        //QProcess::execute(argments);
        QProcess *pro = new QProcess;
        pro->start(argments);
        pro->waitForFinished();
        QString output = pro->readAllStandardError().data();
        QString theResult = output.mid(output.indexOf("Duration:"),18);
        QStringList list = theResult.split(":");
        QString ltemp = list.at(1);
        QString mtemp = list.at(2);
        QString rtemp = list.at(3);
        int total = ltemp.trimmed().toInt()*3600+mtemp.trimmed().toInt()*60+rtemp.trimmed().toInt();
        if(total==0)
        {
            total=1;
        }
        length = total*length;
        //coutA << "the theResult is "<< QString::number(total) << endl;
        //
        objPixmap = QPixmap(picPath);
        temp = new QListWidgetItem((objPixmap.scaled(QSize(60,60),Qt::KeepAspectRatioByExpanding)),tr(""));
        QTimer* timer = new QTimer(this);
        timer->singleShot(10,this,SLOT(videoToPic()));
    }
    else
    {
       objPixmap = QPixmap(content);
       temp = new QListWidgetItem((objPixmap.scaled(QSize(60,60),Qt::KeepAspectRatioByExpanding)),tr(""));
    }
    //test weather to resize the mediaBelow
    if(length+getWidthOfControls(listMediaBelow)>listMediaBelow->width()/2)
    {
        listMediaBelow->resize((length+getWidthOfControls(listMediaBelow))*2,listMediaBelow->height());
        listAudioBelow->resize((length+getWidthOfControls(listMediaBelow))*2,listAudioBelow->height());
        ui->scrollAreaWidgetContents->setFixedWidth(listMediaBelow->width());
    }
    temp->setSizeHint(QSize(length,30));
    temp->setData(2,content);
    temp->setBackgroundColor(QPalette::Base);

    listMediaBelow->addItem(temp);
}

void Dialog::importAudioMethod()
{
    QString content = QFileDialog::getOpenFileName(this, tr("Open Audio"), ".", tr("Audio Files(*.mp3)"));
    if(content.isEmpty()||content.isNull())
    {
        QMessageBox::information(NULL, tr("Open Audio"), tr("You didn't select any files."));
        return;
    }
    //
    QListWidgetItem* temp = new QListWidgetItem(content);
    //get length
    QString argmentsAudio = "ffmpeg -i  " + content;
    QProcess *pro = new QProcess;
    pro->start(argmentsAudio);
    pro->waitForFinished();
    QString output = pro->readAllStandardError().data();
    QString theResult = output.mid(output.indexOf("Duration:"),18);
    QStringList list = theResult.split(":");
    QString ltemp = list.at(1);
    QString mtemp = list.at(2);
    QString rtemp = list.at(3);
    int total = ltemp.trimmed().toInt()*3600+mtemp.trimmed().toInt()*60+rtemp.trimmed().toInt();
    if(total==0)
    {
        total=1;
    }
    int lengthAudio = total*60;
    //test weather to resize the audioBelow
    if(lengthAudio+getWidthOfControls(listAudioBelow)>listAudioBelow->width()/2)
    {
        listAudioBelow->resize((lengthAudio+getWidthOfControls(listAudioBelow))*2,listAudioBelow->height());
        listMediaBelow->resize((lengthAudio+getWidthOfControls(listAudioBelow))*2,listMediaBelow->height());
        ui->scrollAreaWidgetContents->setFixedWidth(listMediaBelow->width());
    }
    //
    temp->setSizeHint(QSize(lengthAudio,30));
    temp->setBackgroundColor(QPalette::Base);
    temp->setData(2,content);
    //coutA << "the data is "<< (temp->data(1)).toString() << endl;
    listAudioBelow->addItem(temp);
}

int Dialog::getWidthOfControls(QListWidget* list)
{
    int width = 0 ;
    for (int i=0;i<list->count();i++)
    {
        width += list->item(i)->sizeHint().width();
    }
    return width;
}

QDockWidget *Dialog::getDockWidget()
{
    dockWidget = ui->dockWidget;
    return dockWidget;
}
