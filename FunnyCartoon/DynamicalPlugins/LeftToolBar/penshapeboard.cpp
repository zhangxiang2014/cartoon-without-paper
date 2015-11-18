#include "penshapeboard.h"
#include <QStyleOption>
#include <QPainter>
#include <QStyle>
#include <QHBoxLayout>
#include <QSlider>
#include <QToolButton>
#include <QPixmap>
#include <QBrush>
#include <QPoint>

static const int MAX_SIZE = 22;
static const int MAX_RADIUS = MAX_SIZE/2 - 1;
static const int WIDTH_SLIDER_MAX_VALUE = 200;
static const int FEATHER_SLIDER_MAX_VALUE = 250;
static const int PEN_WIDTH_SLIDER_MIN_VALUE = \
        1 / double(MAX_RADIUS) * double(WIDTH_SLIDER_MAX_VALUE);

PenShapeBoard::PenShapeBoard(QWidget *parent):
    QWidget(parent),
    m_penShapeBoardVLayout(new QVBoxLayout),
    m_penShapeWidth(WIDTH_SLIDER_MAX_VALUE),
    m_penShapeFeather(FEATHER_SLIDER_MAX_VALUE)
{
    createPenShapeMatrix();
    showSelectedPenShapeSize();
    addChangePenShapeSizeSlider();
    addChangePenShapeFeatherSlider();

    QLabel *showShapeScale = new QLabel(this);//show shape scale
    QHBoxLayout *showScaleHLayout = new QHBoxLayout;
    showShapeScale->setText(". . . . . . . . . . .");
    showScaleHLayout->addWidget(showShapeScale);
    m_penShapeBoardVLayout->addLayout(showScaleHLayout);

    setLayout(m_penShapeBoardVLayout);
    setObjectName("container");
}

PenShapeBoard::~PenShapeBoard()
{
}

void PenShapeBoard::paintEvent(QPaintEvent *)
{
    QStyleOption opt;//make style sheet of custom widget work
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void PenShapeBoard::createPenShapeMatrix()
{
    QVBoxLayout *shapeVLayout = new QVBoxLayout;

    //1.the loop to create a pen shape matrix
    QString iconPath;
    for(int i=0; i<2; i++){
        QHBoxLayout *hLayout = new QHBoxLayout;
        for(int j=0; j<5; j++){
            QToolButton *toolBtn = new QToolButton(this);
            iconPath = ":/images/pen_shapes/pen_shape";
            iconPath += QString::number(5*i+j);
            toolBtn->setIcon(QIcon(iconPath));
            hLayout->addWidget(toolBtn);
        }
        hLayout->setSpacing(0);
        shapeVLayout->addLayout(hLayout);
    }

    shapeVLayout->setSpacing(0);
    m_penShapeBoardVLayout->addLayout(shapeVLayout);
}

void PenShapeBoard::showSelectedPenShapeSize()
{
    m_showPenShape = new QLabel(this);
    QHBoxLayout *showShapeHLayout = new QHBoxLayout;

    //2.show the size of selected pen shape
    QPixmap pix(MAX_SIZE, MAX_SIZE);
    pix.fill(Qt::white);

    QPainter painter(&pix);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setBrush(QBrush(QColor(0, 0, 0, 255)));
    painter.drawEllipse(QPoint(MAX_SIZE/2, MAX_SIZE/2),
                    MAX_RADIUS, MAX_RADIUS);

    m_showPenShape->setPixmap(pix);
    m_showPenShape->setFixedSize(MAX_SIZE, MAX_SIZE);
    showShapeHLayout->addWidget(m_showPenShape);
    m_penShapeBoardVLayout->addLayout(showShapeHLayout);
}

void PenShapeBoard::addChangePenShapeSizeSlider()
{
    QSlider *slider = new QSlider(Qt::Horizontal, this);

    //3.make slider to change the selected shape size
    slider->setMinimum(PEN_WIDTH_SLIDER_MIN_VALUE);
    slider->setMaximum(WIDTH_SLIDER_MAX_VALUE);
    slider->setValue(WIDTH_SLIDER_MAX_VALUE);
    QObject::connect(slider, SIGNAL(valueChanged(int)),
                     this, SLOT(setPenShapeWidth(int)));
    m_penShapeBoardVLayout->addWidget(slider);
}

void PenShapeBoard::addChangePenShapeFeatherSlider()
{
    QSlider *slider = new QSlider(Qt::Horizontal, this);

    //4.make slider to change the selected shape feather
    slider->setMinimum(20);
    slider->setMaximum(FEATHER_SLIDER_MAX_VALUE);
    slider->setValue(FEATHER_SLIDER_MAX_VALUE);
    QObject::connect(slider, SIGNAL(valueChanged(int)),
                     this, SLOT(setPenShapeFeather(int)));
    m_penShapeBoardVLayout->addWidget(slider);
}

/* the slot functions */
void PenShapeBoard::setPenShapeWidth(int width)
{
    m_penShapeWidth = width;
    int radius = MAX_RADIUS * width / WIDTH_SLIDER_MAX_VALUE;
    QPixmap pix(MAX_SIZE, MAX_SIZE);
    pix.fill(Qt::white);

    QPainter painter(&pix);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(0, 0, 0, m_penShapeFeather)));
    painter.setBrush(QBrush(QColor(0, 0, 0, m_penShapeFeather)));
    painter.drawEllipse(QPoint(MAX_SIZE/2, MAX_SIZE/2),
                    radius, radius);

    m_showPenShape->setPixmap(pix);
    emit penShapeWidthChanged(width);
}

void PenShapeBoard::setPenShapeFeather(int feather)
{
    m_penShapeFeather = feather;
    int radius = MAX_RADIUS * m_penShapeWidth / WIDTH_SLIDER_MAX_VALUE;
    QPixmap pix(MAX_SIZE, MAX_SIZE);
    pix.fill(Qt::white);

    QPainter painter(&pix);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(QPen(QColor(0, 0, 0, feather)));
    painter.setBrush(QBrush(QColor(0, 0, 0, feather)));
    painter.drawEllipse(QPoint(MAX_SIZE/2, MAX_SIZE/2),
                    radius, radius);

    m_showPenShape->setPixmap(pix);
    emit penShapeFeatherChanged(feather);
}
