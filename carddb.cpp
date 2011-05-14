#include "carddb.h"
#include <QFile>
#include <QDir>
#include <QXmlStreamReader>
#include <QtGui>

CardDB::CardDB(QObject *parent) :
    QObject(parent)
{

}

void CardDB::empty()
{
    setHash.clear();
    cardHash.clear();
}

void CardDB::loadSetsFromDir(const QString &path)
{
    QDir setsDir(path);
    setsDir.setNameFilters(QStringList("*.xml"));
    QStringList sets = setsDir.entryList();

    for (int i=0; i<sets.size(); i++) {
        QFile file(path+"/"+sets[i]);
        file.open(QIODevice::ReadOnly);
        if (!file.isOpen()) {
            continue;
        }
        QXmlStreamReader xml(&file);
        xml.readNextStartElement();
        if (xml.name() == "kyogo_set")
            loadSetFromXml(xml);
    }
}

// Loads setID and setName only from xml and stores in setHash.
void CardDB::loadSetFromXml(QXmlStreamReader &xml)
{
    QString setID, setName;

    xml.readNextStartElement();
    if (xml.name() == "setID")
        setID = xml.readElementText();
    else
        return;

    xml.readNextStartElement();
    if (xml.name() == "setName")
        setName = xml.readElementText();
    else
        return;

    setHash.insert(setID, new SetInfo(setID,setName));
}

// Loads cardInfo from xml and stores in cardHash.
void CardDB::loadCardsFromXml(QXmlStreamReader &xml)
{
    while (xml.name() != "setID" && !xml.isEndDocument()) {
        xml.readNext();
    }
    QString setID;
    if (xml.name() == "setID")
        setID = xml.readElementText();
    else
        return;

    while (xml.name() != "cards" && !xml.isEndDocument()) {
        xml.readNext();
    }

    if (xml.name() == "cards") {
        while (xml.readNextStartElement()) {
            if (xml.name() == "card") {
                QString name, cost, type, ah, text;
                while (xml.readNextStartElement()) {
                    if (xml.name() == "name")
                        name = xml.readElementText();
                    else if (xml.name() == "cost")
                        cost = xml.readElementText();
                    else if (xml.name() == "type")
                        type = xml.readElementText();
                    else if (xml.name() == "ah")
                        ah = xml.readElementText();
                    else if (xml.name() == "text")
                        text = xml.readElementText();
                }
                cardHash.insert(name, new CardInfo(name,cost,type,ah,text,setID));
            }
        }
    }
}
