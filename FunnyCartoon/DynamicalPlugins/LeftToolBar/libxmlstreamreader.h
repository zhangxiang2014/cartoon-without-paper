#ifndef LIBXMLSTREAMREADER_H
#define LIBXMLSTREAMREADER_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QXmlStreamReader>
#include <QList>
#include "template.h"

class LibXmlStreamReader : public QObject
{
    Q_OBJECT
public:
    explicit LibXmlStreamReader(QWidget *parent = 0);

    bool readXML(const QString &fileName);
    inline QList<Template*> getTemplateList(){ return m_templateList; }

signals:

public slots:

private:
    void readMaterialLibElement();
    void readTemplateElement();
    void skipUnknownElement();

    QWidget *m_parent;
    QXmlStreamReader m_reader;
    QList<Template*> m_templateList;
};

#endif // LIBXMLSTREAMREADER_H
