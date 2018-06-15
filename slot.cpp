#include "slot.h"

Slot::Slot(QString name, QString type, QString value, QString parentName, QObject *parent) : QObject(parent)
{
    _type = type;
    _name = name;
    _value = value;
    added = false;
    _parentName = parentName;
}
