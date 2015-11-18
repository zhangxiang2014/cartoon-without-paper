#include "template.h"

Template::Template(QObject *parent) :
    QObject(parent),
    m_path(""),
    m_size(0)
{
}

Template::Template(QString path, int size, QObject *parent) :
    QObject(parent),
    m_path(path),
    m_size(size)
{
}

