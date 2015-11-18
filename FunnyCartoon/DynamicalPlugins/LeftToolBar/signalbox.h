#ifndef SIGNALBOX_H
#define SIGNALBOX_H

#include <QObject>
#include <QColor>

class SignalBox : public QObject
{
    Q_OBJECT
public:
    explicit SignalBox(QObject *parent = 0);

signals:
    void whichToolsBoardBtnWasClicked(int btnID);
    void colorChanged(QColor);
    void materialImagePathChanged(QString newImagePath);

};

#endif // SIGNALBOX_H
