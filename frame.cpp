#include "frame.h"

Frame::Frame(QString name, QString parentName, QObject *parent) : QObject(parent)
{
    m_name = name;
    this->parentName = parentName;
    added = false;
}

void Frame::setM_Parent(Frame *myParent)
{
    m_parent = myParent;
}

void Frame::addChild(Frame *child)
{
    m_childs.push_back(child);
}

void Frame::addSlot(Slot *slot)
{
    m_slots.push_back(slot);
}

void Frame::deleteChild(QString name)
{
    int i = 0;
    for(Frame *frame : m_childs)
    {
        if(frame->m_name == name)
        {
            m_childs.removeAt(i);
            return;
        }
        i++;
    }
}

void Frame::deleteSlot(QString name)
{
    int i = 0;
    for(Slot *slot : m_slots)
    {
        if(slot->m_name == name)
        {
            m_slots.removeAt(i);
            qDebug() << "ok";
            return;
        }
        i++;
    }
}
