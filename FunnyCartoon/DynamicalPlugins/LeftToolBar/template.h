#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <QObject>
#include <QString>

class Template : public QObject
{
    Q_OBJECT
public:
    explicit Template(QObject *parent=0);
    explicit Template(QString path, int size, QObject *parent=0);

    inline QString getPath(){ return m_path; }
    inline void setPath(QString path) { m_path = path; }
    inline int getSize() { return m_size; }
    inline void setSize(int size) { m_size = size; }

signals:

public slots:

private:
    QString m_path;
    int m_size;

};

#endif // TEMPLATE_H
