#ifndef SETINFO_H
#define SETINFO_H

#include <QObject>

class SetInfo : public QObject
{
    Q_OBJECT

private:
    QString setID;
    QString setName;
    int setNum;
public:
    SetInfo(const QString &_setID = QString(),
            const QString &_setName = QString());
    QString getSetID() const { return setID; }
    QString getSetName() const { return setName; }
    int getSetNum() const { return setNum; }
    void setSetNum(const int &_setNum);
    bool changed;

signals:

public slots:

};

#endif // SETINFO_H
