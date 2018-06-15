#include "frame.h"

Frame::Frame(QString name, QString parentName, QObject *parent) : QObject(parent)
{
    _name = name;
    _parentName = parentName;
    added = false;
}

void Frame::setParent(Frame *myParent)
{
    _parent = myParent;
}

void Frame::addChild(Frame *child)
{
    _childs.push_back(child);
}

void Frame::addSlot(Slot *slot)
{
    _slots.push_back(slot);
}

void Frame::deleteChild(QString name)
{
    int i = 0;
    for(Frame *frame : _childs)
    {
        if(frame->_name == name)
        {
            _childs.removeAt(i);
            return;
        }
        i++;
    }
}

void Frame::deleteSlot(QString name)
{
    int i = 0;
    for(Slot *slot : _slots)
    {
        if(slot->_name == name)
        {
            _slots.removeAt(i);
            qDebug() << "ok";
            return;
        }
        i++;
    }
}
