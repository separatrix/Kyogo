#ifndef SETINFO_H
#define SETINFO_H

#include <QObject>

class SetInfo : public QObject
{
    Q_OBJECT

private:
    QString setID;
    QString setName;
public:
    SetInfo(const QString &_setID = QString(),
            const QString &_setName = QString());
    QString id() const { return setID; }
    QString name() const { return setName; }

signals:

public slots:

};

#endif // SETINFO_H
