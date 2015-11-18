#ifndef MATERIALLIB_H
#define MATERIALLIB_H

#include <QWidget>
#include <QButtonGroup>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QString>
#include "template.h"

class MaterialLib : public QWidget
{
    Q_OBJECT
public:
    explicit MaterialLib(QWidget *parent = 0);
    ~MaterialLib();

    QList<QButtonGroup *> m_libBtnGroupList;
    QString m_currentLibPath;

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);

private:
    void initialTemplateList();
    void addTopIOButton();
    void addCentralShiftBoard();
    void initialShiftBoardButtons(QWidget *parent);
    QString getLibIconPath(const QString &libDirPath);
    void addBottomBoard();
    void addBookBackground(QHBoxLayout *bottomHLayout);
    void addMaterialButtonsBoardScroll(QHBoxLayout *bookBodyHLayout);
    QWidget *getMaterialButtonsBoard(int libSize, const QString &dirPath);
    QPushButton *getMaterialLibBtn(QWidget *parent, int btnId, const QString &dirPath);

    QList<QPushButton*> m_shiftButtonList;
    QButtonGroup *m_currentLibBtnGroup;
    QList<Template*> m_templateList;
    QVBoxLayout *m_libVLayout;
    QStackedWidget *m_boardStack;

private slots:
    void addAMaterialBtnBoard();
    void subAMaterialBtnBoard();

};

#endif // MATERIALLIB_H
