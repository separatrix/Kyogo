#include "setinfo.h"

SetInfo::SetInfo(const QString &_setID, const QString &_setName)
    : setID(_setID), setName(_setName)
{
    this->changed = false;
    this->setNum = 0;
}

void SetInfo::setSetNum(const int &_setNum)
{
    this->setNum = _setNum;
}
