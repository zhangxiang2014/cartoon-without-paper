#ifndef PENSHAPEBOARD_H
#define PENSHAPEBOARD_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class PenShapeBoard : public QWidget
{
    Q_OBJECT
public:
    explicit PenShapeBoard(QWidget *parent = 0);
    ~PenShapeBoard();

signals:
    void penShapeWidthChanged(int newWidth);
    void penShapeFeatherChanged(int newFeather);

public slots:

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void setPenShapeWidth(int width);
    void setPenShapeFeather(int feather);

private:
    void createPenShapeMatrix();
    void showSelectedPenShapeSize();
    void addChangePenShapeSizeSlider();
    void addChangePenShapeFeatherSlider();

    QVBoxLayout *m_penShapeBoardVLayout;
    QLabel *m_showPenShape;
    int m_penShapeWidth;
    int m_penShapeFeather;

};

#endif // PENSHAPEBOARD_H
