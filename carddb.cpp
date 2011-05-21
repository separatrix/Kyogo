#include "carddb.h"
#include "cardinfo.h"
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

QList<CardInfo*> CardDB::getCardList(const int &setNum)
{
    QList<CardInfo*> cardList;
    QHash<int,QString>::const_iterator i = setRef.find(setNum);
    while (i != setRef.end() && i.key() == setNum) {
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

    int setCounter = 1;
    for (int i=0; i<sets.size(); i++) {
        QFile file(path+"/"+sets[i]);        
        if (!file.open(QIODevice::ReadOnly))
            continue;        
        QXmlStreamReader xml(&file);
        xml.readNextStartElement();
        if (xml.name() == "kyogo_set") {
            if (loadSetFromXml(xml, setCounter)) {
                pathHash.insert(setCounter, path+"/"+sets[i]);
                setCounter++;
            }
        }
    }
}

bool CardDB::loadSetFromXml(QXmlStreamReader &xml, const int setCounter)
{
    QString setID, setName;

    while (xml.name() != "setID" && !xml.isEndDocument()) {
        xml.readNext();
    }

    if (xml.name() == "setID")
        setID = xml.readElementText();
    else
        return false;

    xml.readNextStartElement();
    if (xml.name() == "setName")
        setName = xml.readElementText();
    else
        return false;

    setHash.insert(setCounter, new SetInfo(setID,setName));
    setHash.value(setCounter)->setSetNum(setCounter);

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
                QString cardID = setCounter + name;
                cardHash.insert(cardID, new CardInfo(name,cost,type,ah,text,setID));
                setRef.insertMulti(setCounter, cardID);
            }
        }
    }
    return true;
}

bool CardDB::idExists(const QString &setID)
{
    QHash<int, SetInfo*>::const_iterator i;
    for (i = setHash.constBegin(); i != setHash.constEnd(); i++) {
        if (i.value()->getSetID() == setID)
            return true;
    }
    return false;
}

void CardDB::updateSet(const int &setNum, const QString &setID, const QString &setName)
{
    setHash.remove(setNum);
    setHash.insert(setNum, new SetInfo(setID, setName));
    setHash.value(setNum)->changed = true;
}

void CardDB::writeChanges()
{
    QHash<int, SetInfo*>::const_iterator i;
    for (i = setHash.constBegin(); i != setHash.constEnd(); i++) {
        if (i.value()->changed) {
            QFile file(pathHash.value(i.key()));
            if (file.open(QIODevice::WriteOnly)) {
                QXmlStreamWriter xml(&file);

                xml.setAutoFormatting(true);
                xml.writeStartDocument();
                xml.writeStartElement("kyogo_set");
                xml.writeTextElement("setID", i.value()->getSetID());
                xml.writeTextElement("setName", i.value()->getSetName());
                xml.writeStartElement("cards");
                QList<CardInfo*> cardList = this->getCardList(i.key());
                for (int u=0; u<cardList.size(); u++) {
                    xml.writeStartElement("card");
                    xml.writeTextElement("name", cardList[u]->getName());
                    xml.writeTextElement("cost", cardList[u]->getCost());
                    xml.writeTextElement("type", cardList[u]->getType());
                    xml.writeTextElement("ah", cardList[u]->getAh());
                    xml.writeTextElement("text", cardList[u]->getText());
                    xml.writeEndElement();
                }
                xml.writeEndElement();
                xml.writeEndElement();
                xml.writeEndDocument();
            }
        }
    }
}
