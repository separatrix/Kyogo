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

QList<CardInfo*> CardDB::getCardList(const QString &setID)
{
    QList<CardInfo*> cardList;
    QHash<QString,QString>::const_iterator i = setRef.find(setID);
    while (i != setRef.end() && i.key() == setID) {
        cardList << cardHash.value(i.value());
        i++;
    }
    return cardList;
}

void CardDB::loadSetsFromDir(const QString &path)
{
    setsDir = QDir(path);
    setsDir.setNameFilters(QStringList("*.xml"));
    QStringList sets = setsDir.entryList();

    for (int i=0; i<sets.size(); i++) {
        QFile file(path+"/"+sets[i]);        
        if (!file.open(QIODevice::ReadOnly))
            continue;        
        QXmlStreamReader xml(&file);
        xml.readNextStartElement();
        if (xml.name() == "kyogo_set")
            QString setID = loadSetFromXml(xml);
            if (setID != "")
                fileHash.insert(setID, file);
    }
}

// Loads setID and setName and cards from xml and stores in setHash/cardHash.
QString CardDB::loadSetFromXml(QXmlStreamReader &xml)
{
    QString setID, setName;

    while (xml.name() != "setID" && !xml.isEndDocument()) {
        xml.readNext();
    }

    if (xml.name() == "setID")
        setID = xml.readElementText();
    else
        return QString();

    xml.readNextStartElement();
    if (xml.name() == "setName")
        setName = xml.readElementText();
    else
        return QString();

    setHash.insert(setID, new SetInfo(setID,setName));

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
                QString cardID = setID + name;
                cardHash.insert(cardID, new CardInfo(name,cost,type,ah,text,setID));
                setRef.insertMulti(setID, cardID);
            }
        }
        return setID;
    }
}

void CardDB::updateSetName(const QString &setID, const QString &setName)
{
    if (setHash.value(setID) != setName) {
        setHash.remove(setID);
        setHash.insert(setID,new SetInfo(setID, setName));
        setHash.value(setID)->changed = true;
    }
}

void CardDB::writeChanges()
{
    QHash<QString, SetInfo*>::const_iterator i;
    for (i = setHash.constBegin(); i != fileHash.constEnd(); i++) {
        if (i.value()->changed) {
            QFile file = fileHash.value(i.key());
            if (file.open(QIODevice::WriteOnly)) {
                QXmlStreamWriter xml(&file);

                xml.setAutoFormatting(true);
                xml.writeStartDocument();
            }
        }
    }
}
