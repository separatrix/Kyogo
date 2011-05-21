#ifndef CARDDB_H
#define CARDDB_H

#include <QObject>
#include <QXmlStreamReader>
#include <QHash>
#include <QDir>
#include "cardinfo.h"
#include "setinfo.h"

class CardDB : public QObject
{
    Q_OBJECT
public:
    explicit CardDB(QObject *parent = 0);
    void empty();
    void loadSetsFromDir(const QString &path);
    QList<SetInfo*> getSetList() const { return setHash.values(); }
    QList<CardInfo*> getCardList() const { return cardHash.values(); }
    QList<CardInfo*> getCardList(const int &setNum);
    QString getSetID(const int &setNum) const { return setHash.value(setNum)->getSetID(); }
    bool idExists(const QString &setID);
    void updateSet(const int &setNum, const QString &setID, const QString &setName);
    void writeChanges();

private:
    // key: cardID(setNum+cardName)
    QHash<QString, CardInfo*> cardHash;
    // key: setNum
    QHash<int, SetInfo*> setHash;
    // key: setNum; val: cardID (multi)
    QHash<int, QString> setRef;
    // key: setNum; val: path to set XML
    QHash<int, QString> pathHash;
    QDir setsDir;
    bool loadSetFromXml(QXmlStreamReader &xml, const int setCounter);
};

#endif // CARDDB_H
