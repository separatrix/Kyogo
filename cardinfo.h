#ifndef CARDINFO_H
#define CARDINFO_H

#include <QObject>

class CardInfo : public QObject
{
    Q_OBJECT

private:
    QString name;
    QString cost;
    QString type;
    QString ah;
    QString text;
    QString setID;

public:
    CardInfo(const QString &_name = QString(),
             const QString &_cost = QString(),
             const QString &_type = QString(),
             const QString &_ah = QString(),
             const QString &_text = QString(),
             const QString &_setID = QString());

signals:

public slots:

};

#endif // CARDINFO_H
