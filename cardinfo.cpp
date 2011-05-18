#include "cardinfo.h"

CardInfo::CardInfo(const QString &_name,
                   const QString &_cost,
                   const QString &_type,
                   const QString &_ah,
                   const QString &_text,
                   const QString &_setID)
    : name(_name), cost(_cost), type(_type), ah(_ah), text(_text), setID(_setID)
{
}
