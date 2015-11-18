#include "materiallib.h"
#include "libxmlstreamreader.h"
#include <QStyleOption>
#include <QPainter>
#include <QStyle>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QIcon>
#include <QSize>
#include <QDir>
#include <QMessageBox>
#include <QDebug>

MaterialLib::MaterialLib(QWidget *parent):
    QWidget(parent),
    m_currentLibBtnGroup(new QButtonGroup(this)),
    m_currentLibPath(""),
    m_libVLayout(new QVBoxLayout(this))
{
    initialTemplateList();
    addTopIOButton();
    addCentralShiftBoard();
    addBottomBoard();

    setMinimumSize(150, 400);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    setFixedWidth(180);
}

MaterialLib::~MaterialLib()
{
}

void MaterialLib::paintEvent(QPaintEvent *)
{
    QStyleOption opt;//make style sheet of custom widget work
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void MaterialLib::initialTemplateList()
{
    //parse material library xml file before import the material resources
    LibXmlStreamReader libXmlReader(this);
    QString templateXmlPath = QDir::currentPath();
    templateXmlPath += "/resources/templates.xml";
    if(libXmlReader.readXML(templateXmlPath)){
        m_templateList = libXmlReader.getTemplateList();
        m_currentLibPath = m_templateList.at(0)->getPath();
    }
    else{//if cannot import the material resource, the program will exit with 1 code
        exit(1);//TODO:Is there a problem if force the program to exit
    }
}

void MaterialLib::addTopIOButton()
{
    //1.the top
    QHBoxLayout *ioTopHLayout = new QHBoxLayout;
    QPushButton *bookIn = new QPushButton(this);
    QPushButton *bookOut = new QPushButton(this);

    bookIn->setIcon(QIcon(":/images/plus_circle"));//the plus button
    bookIn->setObjectName("bookIn");
    bookIn->setFixedSize(25, 25);
    ioTopHLayout->addWidget(bookIn);

    bookOut->setIcon(QIcon(":/images/minus_circle"));//the minus button
    bookOut->setObjectName("bookOut");
    bookOut->setFixedSize(25, 25);
    ioTopHLayout->addWidget(bookOut);
    m_libVLayout->addLayout(ioTopHLayout);
}

void MaterialLib::addCentralShiftBoard()
{
    //2.the center: shift material library board
    QHBoxLayout *shiftCenterHLayout = new QHBoxLayout;

    QPushButton *leftArrow = new QPushButton(this);//the left arrow
    leftArrow->setObjectName("leftArrowButton");
    QObject::connect(leftArrow, SIGNAL(clicked(bool)),
                     this, SLOT(subAMaterialBtnBoard()));
    shiftCenterHLayout->addWidget(leftArrow);

    QWidget *shiftBoard = new QWidget(this);//the central shift board
    shiftBoard->setObjectName("shiftBoard");
    shiftBoard->setFixedSize(110, 60);
    QHBoxLayout *shiftBoardHLayout = new QHBoxLayout;

    initialShiftBoardButtons(shiftBoard);
    shiftBoardHLayout->addWidget(m_shiftButtonList.at(0));//central left book button
    shiftBoardHLayout->addWidget(m_shiftButtonList.at(1));//central center book button
    shiftBoardHLayout->addWidget(m_shiftButtonList.at(2));//central right book button
    shiftBoard->setLayout(shiftBoardHLayout);
    shiftCenterHLayout->addWidget(shiftBoard);

    QPushButton *rightArrow = new QPushButton(this);//the right arrow
    rightArrow->setObjectName("rightArrowButton");
    QObject::connect(rightArrow, SIGNAL(clicked(bool)),
                     this, SLOT(addAMaterialBtnBoard()));
    shiftCenterHLayout->addWidget(rightArrow);

    m_libVLayout->addLayout(shiftCenterHLayout);
}

void MaterialLib::initialShiftBoardButtons(QWidget *parent)
{
    QPushButton *libBtnLeft = new QPushButton(parent);
    libBtnLeft->setFixedSize(30, 40);
    libBtnLeft->setIcon(QIcon(getLibIconPath(m_templateList.at(1)->getPath())));
    libBtnLeft->setIconSize(QSize(20, 20));
    libBtnLeft->setStyleSheet("border-image: url(:/images/book_disabled);");
    libBtnLeft->setEnabled(false);
    m_shiftButtonList.append(libBtnLeft);

    QPushButton *libBtnCenter = new QPushButton(parent);
    libBtnCenter->setFixedSize(30, 40);
    libBtnCenter->setIcon(QIcon(getLibIconPath(m_templateList.at(0)->getPath())));
    libBtnCenter->setIconSize(QSize(20, 20));
    libBtnCenter->setStyleSheet("border-image: url(:/images/book_sys);");
    m_shiftButtonList.append(libBtnCenter);

    QPushButton *libBtnRight = new QPushButton(parent);
    libBtnRight->setFixedSize(30, 40);
    libBtnRight->setIcon(QIcon(":/images/empty_lib"));
    libBtnRight->setStyleSheet("border-image: url(:/images/book_disabled);");
    libBtnRight->setEnabled(false);
    m_shiftButtonList.append(libBtnRight);
}

QString MaterialLib::getLibIconPath(const QString &libDirPath)
{
    QString libIconPath = QDir::currentPath();
    libIconPath += "/";
    libIconPath += libDirPath;
    libIconPath += "/0.png";
    return libIconPath;
}

void MaterialLib::addBottomBoard()
{
    //3.the bottom
    QHBoxLayout *bottomHLayout = new QHBoxLayout;
    QVBoxLayout *bottomInnerVLayout = new QVBoxLayout;

    addBookBackground(bottomHLayout);//add a book as background of material buttons board

    QPushButton *doubleLeftArrow = new QPushButton(this);//double left arrow
    doubleLeftArrow->setObjectName("doubleLeftArrowButton");
    bottomInnerVLayout->addWidget(doubleLeftArrow);

    QPushButton *doubleRightArrow = new QPushButton(this);//double right arrow
    doubleRightArrow->setObjectName("doubleRightArrowButton");
    bottomInnerVLayout->addWidget(doubleRightArrow);

    bottomHLayout->addLayout(bottomInnerVLayout);
    m_libVLayout->addLayout(bottomHLayout);
}

void MaterialLib::addBookBackground(QHBoxLayout *bottomHLayout)
{
    //create a book background
    QVBoxLayout *bookVLayout = new QVBoxLayout;
    QWidget *bookHeadBackground = new QWidget(this);//the head of book
    bookHeadBackground->setObjectName("bookHeadBackground");
    bookHeadBackground->setFixedHeight(10);
    bookVLayout->addWidget(bookHeadBackground);

    QWidget *bookBodyBackground = new QWidget(this);//the body of book
    QHBoxLayout *bookBodyHLayout = new QHBoxLayout;
    bookBodyBackground->setObjectName("bookBodyBackground");
    bookVLayout->addWidget(bookBodyBackground);

    QWidget *bookFootBackground = new QWidget(this);//the foot of book
    bookFootBackground->setObjectName("bookFootBackground");
    bookFootBackground->setFixedHeight(10);
    bookVLayout->addWidget(bookFootBackground);
    bookVLayout->setSpacing(0);

    addMaterialButtonsBoardScroll(bookBodyHLayout);//add material buttons board in book

    bookBodyBackground->setLayout(bookBodyHLayout);
    bottomHLayout->addLayout(bookVLayout);
}

void MaterialLib::addMaterialButtonsBoardScroll(QHBoxLayout *bookBodyHLayout)
{
    //add material library bottons board in book body background widget
    QScrollArea *btnBoardScroll = new QScrollArea(this);
    m_boardStack = new QStackedWidget(this);
    m_boardStack->setObjectName("materialBoardStack");

    //TODO: run a new thread to import the material library
    int libSize = 0;
    for(int j=0; j<m_templateList.size(); j++){//import material libraries
        m_currentLibBtnGroup = new QButtonGroup(this);
        libSize = m_templateList.at(j)->getSize();
        m_boardStack->addWidget(getMaterialButtonsBoard(libSize,
                                    m_templateList.at(j)->getPath()));
        m_libBtnGroupList.append(m_currentLibBtnGroup);
    }

    btnBoardScroll->setObjectName("materialBtnScroll");
    btnBoardScroll->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    btnBoardScroll->setFixedWidth(120);
    btnBoardScroll->setWidget(m_boardStack);

    bookBodyHLayout->addWidget(btnBoardScroll);
}

QWidget *MaterialLib::getMaterialButtonsBoard(int libSize, const QString &dirPath)
{
    const int btnHeight = 35;
    int boardHeight = (libSize/2)*btnHeight;
    QWidget *btnBoard = new QWidget(this);
    QVBoxLayout *btnBoardVLayout = new QVBoxLayout;

    for(int i=0; i<libSize/2; i++){
        QHBoxLayout *lineHLayout = new QHBoxLayout;
        lineHLayout->addWidget(getMaterialLibBtn(btnBoard, 2*i, dirPath));
        lineHLayout->addWidget(getMaterialLibBtn(btnBoard, 2*i+1, dirPath));
        btnBoardVLayout->addLayout(lineHLayout);
    }

    if(libSize%2 != 0){//if libSize is odd number
        QHBoxLayout *lineHLayout = new QHBoxLayout;//the last lib button
        QPushButton *spacer = new QPushButton(btnBoard);
        lineHLayout->addWidget(getMaterialLibBtn(btnBoard, libSize-1, dirPath));

        spacer->setObjectName("materialButton");//the spacer to take up extra space
        lineHLayout->addWidget(spacer);
        btnBoardVLayout->addLayout(lineHLayout);
        boardHeight += btnHeight;
    }

    btnBoard->setObjectName("materialBtnBoard");
    btnBoard->setLayout(btnBoardVLayout);
    btnBoard->setFixedSize(95, boardHeight);
    return btnBoard;
}

QPushButton *MaterialLib::getMaterialLibBtn(QWidget *parent, int btnId,
                                            const QString &dirPath)
{
    QPushButton *libBtn = new QPushButton(parent);
    QString btnIconStyle("border-image: url(");

    btnIconStyle += QDir::currentPath();
    btnIconStyle += "/";
    btnIconStyle += dirPath;
    btnIconStyle += "/";
    btnIconStyle += QString::number(btnId+1);
    btnIconStyle += ".png";
    btnIconStyle += ");";

    libBtn->setObjectName("materialButton");
    libBtn->setStyleSheet(btnIconStyle);
    libBtn->setCheckable(true);

    m_currentLibBtnGroup->addButton(libBtn, btnId);
    return libBtn;
}

/* the slot functions */
void MaterialLib::addAMaterialBtnBoard()
{
    int addIndex = m_boardStack->currentIndex() + 1;
    if(addIndex < m_templateList.size()){
        m_shiftButtonList.at(2)->setIcon(QIcon(getLibIconPath(m_currentLibPath)));//right
        m_currentLibPath = m_templateList.at(addIndex)->getPath();
        m_shiftButtonList.at(1)->setIcon(QIcon(getLibIconPath(m_currentLibPath)));//center
        if((addIndex+1) == m_templateList.size()){//left
            m_shiftButtonList.at(0)->setIcon(QIcon(":/images/empty_lib"));
        }
        else{
            m_shiftButtonList.at(0)->setIcon(QIcon(getLibIconPath(
                                 m_templateList.at(addIndex+1)->getPath())));
        }
        m_boardStack->setCurrentIndex(addIndex);
    }else{
        QMessageBox::information(this, "Info", "The end.");
    }
}

void MaterialLib::subAMaterialBtnBoard()
{
    int subIndex = m_boardStack->currentIndex() - 1;
    if(subIndex < 0){
        QMessageBox::information(this, "Info", "The end.");
    }else{
        m_shiftButtonList.at(0)->setIcon(QIcon(getLibIconPath(m_currentLibPath)));//left
        m_currentLibPath = m_templateList.at(subIndex)->getPath();
        m_shiftButtonList.at(1)->setIcon(QIcon(getLibIconPath(m_currentLibPath)));//center
        if((subIndex-1) == -1){//right
            m_shiftButtonList.at(2)->setIcon(QIcon(":/images/empty_lib"));
        }
        else{
            m_shiftButtonList.at(2)->setIcon(QIcon(getLibIconPath(
                                 m_templateList.at(subIndex-1)->getPath())));
        }
        m_boardStack->setCurrentIndex(subIndex);
    }
}
