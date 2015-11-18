#ifndef TOPSIGNALBOX_H
#define TOPSIGNALBOX_H

#include <QObject>

class TopSignalBox : public QObject
{
    Q_OBJECT
public:
    explicit TopSignalBox(QObject *parent = 0):QObject(parent){}

signals:
    void whichTopToolBarBtnWasClicked(int btnID);

};

#endif // TOPSIGNALBOX_H
