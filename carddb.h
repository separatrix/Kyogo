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
    QList<CardInfo*> getCardList(const QString &setID);
    bool idExists(const QString &setID) const { return setHash.contains(setID); }
    void updateSetName(const QString &setID, const QString &setName);
    void writeChanges();

private:
    QHash<QString, CardInfo*> cardHash;
    QHash<QString, SetInfo*> setHash;
    QHash<QString, QString> setRef;
    QHash<QString, QFile> fileHash;
    QDir setsDir;
    QString loadSetFromXml(QXmlStreamReader &xml);
};

#endif // CARDDB_H
