#ifndef LEFTSIGNALBOX_H
#define LEFTSIGNALBOX_H

#include <QObject>
#include <QColor>

class LeftSignalBox : public QObject
{
    Q_OBJECT
public:
    explicit LeftSignalBox(QObject *parent = 0):QObject(parent){}

signals:
    void whichToolsBoardBtnWasClicked(int btnID);
    void colorChanged(QColor);
    void materialImagePathChanged(QString newImagePath);
    void penWidthChanged(int newWidth);
    void penFeatherChanged(int newFeather);

};

#endif // LEFTSIGNALBOX_H
