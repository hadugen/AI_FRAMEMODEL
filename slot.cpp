#include "slot.h"

Slot::Slot(QString name, QString type, QString value, QString parentName, QObject *parent) : QObject(parent)
{
    m_type = type;
    m_name = name;
    m_value = value;
    added = false;
    this->parentName = parentName;
}
