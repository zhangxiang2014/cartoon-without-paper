#include "colorbar.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QStyle>
#include <QButtonGroup>
#include <QColorDialog>

ColorBar::ColorBar(bool isHorizontal, QWidget *parent):
    QWidget(parent)
{
    //initial color array
    if(isHorizontal)
        m_layout = new QHBoxLayout(this);
    else
        m_layout = new QVBoxLayout(this);
    initialColorArray();
    addColorButtons();
    addMixColorButton();
    setObjectName("container");
}

ColorBar::~ColorBar()
{
}

void ColorBar::initialColorArray()
{
    m_colorArray[0] = QColor(0, 0, 0);//black
    m_colorArray[1] = QColor(255, 0, 0);//red
    m_colorArray[2] = QColor(0, 255, 0);//green
    m_colorArray[3] = QColor(0, 0, 255);//blue
    m_colorArray[4] = QColor(255, 255, 0);//yellow
    m_colorArray[5] = QColor(0, 255, 255);//water blue
    m_colorArray[6] = QColor(255, 0, 255);//pink
    m_colorArray[7] = QColor(128, 128, 128);//gray
}

void ColorBar::paintEvent(QPaintEvent *)
{
    QStyleOption opt;//make style sheet of custom widget work
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void ColorBar::addColorButtons()
{
    //TODO: button group instead m_colorBtnArray
    QButtonGroup *btnGroup = new QButtonGroup(this);

    //the first color button which is checked
    QPushButton *checkedColorBtn = new QPushButton(this);
    checkedColorBtn->setObjectName("checkedColorBtn");
    checkedColorBtn->setStyleSheet(getCheckedColorButtonStyleSheet(m_colorArray[0]));
    m_layout->addWidget(checkedColorBtn);
    m_colorBtnArray[0] = checkedColorBtn;

    for(int i=1; i<s_ColorButtonNum; i++){//the other color buttons
        QPushButton *colorBtn = new QPushButton(this);
        colorBtn->setObjectName("colorButton");
        colorBtn->setStyleSheet(getColorButtonStyleSheet(m_colorArray[i]));
        m_colorBtnArray[i] = colorBtn;
        btnGroup->addButton(colorBtn, i);
        m_layout->addWidget(colorBtn);
    }

    QObject::connect(btnGroup, SIGNAL(buttonClicked(int)),
                     this, SLOT(colorButtonClicked(int)));
}

void ColorBar::addMixColorButton()
{
    QPushButton *mixColorBtn = new QPushButton(this);
    mixColorBtn->setObjectName("mixColorButton");
    m_layout->addWidget(mixColorBtn);

    QObject::connect(mixColorBtn, SIGNAL(clicked(bool)),
                     this, SLOT(mixColorButtonClicked()));
}

QString ColorBar::getCheckedColorButtonStyleSheet(QColor color)
{
    QString styleStr("background-color: qradialgradient(cx:0.5, cy:0.5, radius:1,\
                     fx:0.5, fy:0.5, stop:0 rgb(");
    styleStr += QString::number(color.red());
    styleStr += ", ";
    styleStr += QString::number(color.green());
    styleStr += ", ";
    styleStr += QString::number(color.blue());
    styleStr += "), stop:1 white);";
    return styleStr;
}

QString ColorBar::getColorButtonStyleSheet(QColor color)
{
    QString styleStr("background-color: qradialgradient(cx:0.6, cy:0.4, radius:0.3,\
                     fx:0.6, fy:0.4, stop:0 white, stop:1.0 rgb(");
    styleStr += QString::number(color.red());
    styleStr += ", ";
    styleStr += QString::number(color.green());
    styleStr += ", ";
    styleStr += QString::number(color.blue());
    styleStr += "));";
    return styleStr;
}

/* the slot functions */
void ColorBar::mixColorButtonClicked()
{
    QColor color = QColorDialog::getColor(static_cast<int>(Qt::black), this);
    if(color.isValid()){
        //update the color of color buttons TODO: improve the method
        for(int i=s_ColorButtonNum-1; i>0; i--){
            m_colorArray[i] = m_colorArray[i-1];
            m_colorBtnArray[i]->setStyleSheet(getColorButtonStyleSheet(m_colorArray[i-1]));
        }
        m_colorArray[0] = color;
        m_colorBtnArray[0]->setStyleSheet(getCheckedColorButtonStyleSheet(color));

        emit currentColorChanged(color);
    }
}

void ColorBar::colorButtonClicked(int btnID)
{
    QColor color = m_colorArray[btnID];
    for(int i=btnID; i>0; i--){
        m_colorArray[i] = m_colorArray[i-1];
        m_colorBtnArray[i]->setStyleSheet(getColorButtonStyleSheet(m_colorArray[i-1]));
    }
    m_colorArray[0] = color;
    m_colorBtnArray[0]->setStyleSheet(getCheckedColorButtonStyleSheet(color));

    emit currentColorChanged(color);
}
