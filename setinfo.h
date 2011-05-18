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
    QString getSetID() const { return setID; }
    QString getSetName() const { return setName; }
    bool changed = false;

signals:

public slots:

};

#endif // SETINFO_H
