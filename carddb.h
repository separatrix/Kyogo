#ifndef CARDDB_H
#define CARDDB_H

#include <QObject>
#include <QXmlStreamReader>
#include <QHash>
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

private:
    QHash<QString, CardInfo*> cardHash;
    QHash<QString, SetInfo*> setHash;
    void loadSetFromXml(QXmlStreamReader &xml);
    void loadCardsFromSet(const QString &setID);
};

#endif // CARDDB_H
