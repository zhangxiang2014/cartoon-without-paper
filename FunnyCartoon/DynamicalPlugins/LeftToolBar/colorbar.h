#ifndef COLORBAR_H
#define COLORBAR_H

#include <QWidget>
#include <QString>
#include <QColor>
#include <QBoxLayout>
#include <QPushButton>

class ColorBar : public QWidget
{
    Q_OBJECT
public:
    explicit ColorBar(bool isHorizontal, QWidget *parent = 0);
    ~ColorBar();

    //Size is the size of width when color bar is vertical
    //or height when color bar is horizontal
    static const int SIZE = 40;

signals:
    void currentColorChanged(QColor newColor);

public slots:

protected:
    void paintEvent(QPaintEvent *);

private:
    void initialColorArray();
    void addColorButtons();
    void addMixColorButton();
    QString getColorButtonStyleSheet(QColor color);
    QString getCheckedColorButtonStyleSheet(QColor color);

    static const int s_ColorButtonNum = 8;//the number of color buttons
    QBoxLayout *m_layout;
    QPushButton *m_colorBtnArray[s_ColorButtonNum];
    QColor m_colorArray[s_ColorButtonNum];

private slots:
    void mixColorButtonClicked();
    void colorButtonClicked(int btnID);

};

#endif // COLORBAR_H
