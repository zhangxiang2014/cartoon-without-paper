#include "mainwindow.h"
#include <QApplication>
#include <QString>
#include <QFile>
#include <QTextStream>

void loadQssFile(QApplication &app, const QString &qssFileName);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    loadQssFile(app, ":/qss/skin");

    MainWindow w;
    w.show();

    return app.exec();
}

//load qss file to change native UI of the application
static void loadQssFile(QApplication &app, const QString &qssFileName){
    QFile qssFile(qssFileName);
    QString qssContent("");

    if(qssFile.open(QFile::ReadOnly)){
        QTextStream iStream(&qssFile);
        qssContent = iStream.readAll();
        qssFile.close();
    }

    app.setStyleSheet(qssContent);
}
