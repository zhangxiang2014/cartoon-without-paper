
#include <QLabel>
#include <QSlider>
#include <QGridLayout>
#include <QLocale>
#include <cmath>
#include "spinslider.h"

SpinSlider::SpinSlider(QString text, QString type, QString dataType, qreal min, qreal max, QWidget* parent) : QWidget(parent)
{
    value = 1.0;
    this->type = type;
    this->dataType = dataType;
    this->min = min;
    this->max = max;
    QLabel* label = new QLabel(text+": ");
    label->setFont( QFont("Helvetica", 10) );
    valueLabel = new QLabel("--");
    valueLabel->setFont( QFont("Helvetica", 10) );//这个是设计字体大小的
    //valueLabel->setFixedWidth(30);
    valueLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    slider = new QSlider(Qt::Horizontal, this);
    slider->setMinimum(0);
    slider->setMaximum(100);
//    slider->setStyleSheet( "QSlider::add-page:vertical {background: url(:icons/b2.jpg) ;};"

//                           "QSlider::sub-page:vertical {background: pink;}");
    slider->setStyleSheet(
                "QSlider::groove:horizontal {                                "
                "     border: 1px solid #999999;                             "
                "     height: 2px;                                           "
                "     margin: 0px 0;                                         "
                "     left: 7px; right: 7px;                               "
                " }                                                          "
                "QSlider::handle:horizontal {                                "
                "     border: 1px solid #5c5c5c;                             "
                " border-image:url(:icons/test2.png);"
                "     width: 18px;                                           "
                "     margin: -12px -7px -12px -7px;                           "
                " }                                                          "
                "                                                            "
                "QSlider::sub-page:horizontal{                               "
                " background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(27, 5, 27, 255), stop:0.25 rgba(99, 20, 102, 255), stop:0.5 rgba(154, 30, 158, 255), stop:1 rgba(173, 57, 176, 255));                      "
                "}                                                           "
                "QSlider::add-page:horizontal{                               "
                " background-image:url(:icons/test2.png)   "
                "}"
                );
    slider->setMaximumWidth(250);//用来修改滑块长度的
    slider->setMaximumHeight(100);
   // slider->setStyleSheet("background:transparent;");//设置与父类背景一致
    //slider->setFixedHeight(16);
    QGridLayout* lay = new QGridLayout();
    lay->setMargin(2);
    lay->setSpacing(5);
    //lay->setColumnStretch(0,1);
    lay->addWidget(label,0,0,1,1);
    lay->addWidget(valueLabel,0,1,1,1);
    lay->addWidget(slider,1,0,1,2);
    setLayout(lay);
    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(changeValue(int)));
    connect(slider, SIGNAL(sliderReleased()), this, SLOT(sliderReleased()));
    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(sliderMoved(int)));
}

void SpinSlider::changeValue(qreal value)
{
    this->value = value;
    if (dataType == "integer")
    {
        valueLabel->setText( QString::number(qRound(value)) );
    }
    else
    {
        valueLabel->setText( QLocale::system().toString(value,'f',1) );
    }
}

void SpinSlider::changeValue(int value)
{
    qreal value2 = 0.0;
    if (type=="linear") value2 = min + value*(max-min)/100;
    if (type=="log") value2 = min * std::exp( value*std::log(max/min) / 100 );
    changeValue(value2);
}

void SpinSlider::setValue(qreal value)
{
    qreal value2 = 0.0;
    if (type=="linear") value2 = qRound( 100*(value-min)/(max-min) );
    if (type=="log") value2 = qRound( 100.0*std::log(value/0.2)/log(1000.0) );
    slider->setSliderPosition(value2);
    changeValue(value);
}

void SpinSlider::sliderReleased()
{
    //qDebug() << "sliderReleased";
    emit valueChanged(this->value);
}

void SpinSlider::sliderMoved(int value)
{
    changeValue(value);
    // Don't update value while the slider is still moving.
    //emit valueChanged(this->value);
}
