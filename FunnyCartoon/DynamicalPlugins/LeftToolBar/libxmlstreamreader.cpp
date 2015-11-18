#include "libxmlstreamreader.h"
#include <QFile>
#include <QMessageBox>

LibXmlStreamReader::LibXmlStreamReader(QWidget *parent) :
    QObject(parent),
    m_parent(parent)
{
}

bool LibXmlStreamReader::readXML(const QString &fileName)
{
    QFile xmlFile(fileName);
    if(!xmlFile.open(QFile::ReadOnly | QFile::Text)){
        QString errorTip("Cannot read file : ");
        QMessageBox::critical(m_parent, "Error", errorTip+=fileName);
        return false;
    }

    m_reader.setDevice(&xmlFile);
    while(!m_reader.atEnd()){//parse the xml file
        if(m_reader.isStartElement()){
            if(m_reader.name() == "material-lib"){
                readMaterialLibElement();
            }else{
                m_reader.raiseError("Not a valid templates file");
            }
        }else{
            m_reader.readNext();
        }
    }

    xmlFile.close();
    if(m_reader.hasError()){//error handling
        QString errorTip("Failed to parse file : ");
        QMessageBox::critical(m_parent, "Error", errorTip+=fileName);
        return false;
    }else if(xmlFile.error() != QFile::NoError){
        QString errorTip("Cannot read file : ");
        QMessageBox::critical(m_parent, "Error", errorTip+=fileName);
        return false;
    }
    return true;
}

void LibXmlStreamReader::readMaterialLibElement()
{
    m_reader.readNext();
    while(!m_reader.atEnd()){
        if(m_reader.isEndElement()){
            m_reader.readNext();
            break;
        }

        if(m_reader.isStartElement()){
            if(m_reader.name() == "template"){
                readTemplateElement();
            }else{
                skipUnknownElement();
            }
        }else{
            m_reader.readNext();
        }
    }
}

void LibXmlStreamReader::readTemplateElement()
{
    //TODO: there is a problem when designed this class
    Template *temp = new Template(m_parent);
    temp->setPath(m_reader.attributes().value("path").toString());
    temp->setSize(m_reader.attributes().value("size").toInt());
    m_templateList.append(temp);

    m_reader.readNext();
    while(!m_reader.atEnd()){
        if(m_reader.isEndElement()){
            m_reader.readNext();
            break;
        }

        if(m_reader.isStartElement()){
            skipUnknownElement();
        }else{
            m_reader.readNext();
        }
    }
}

void LibXmlStreamReader::skipUnknownElement()
{
    m_reader.readNext();
    while(!m_reader.atEnd()){
        if(m_reader.isEndElement()){
            m_reader.readNext();
            break;
        }

        if(m_reader.isStartElement()){
            skipUnknownElement();
        }else{
            m_reader.readNext();
        }
    }
}
