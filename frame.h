#ifndef FRAME_H
#define FRAME_H
#include <QObject>
#include <slot.h>
#include <QDebug>
class Slot;

class Frame : public QObject
{
    Q_OBJECT

public:
    explicit Frame(QString name, QString parentName, QObject *parent = nullptr);
    void addChild(Frame *child);
    void deleteChild(QString name);
    void addSlot(Slot *slot);
    void deleteSlot(QString name);
    void setM_Parent(Frame *myParent);
    void setParent(Frame *myParent);
    QString _name, _parentName;
    QList <Frame*> _childs;
    QList <Slot*> _slots;
    Frame* _parent;
    bool added;


signals:

public slots:

private:

};

#endif // FRAME_H
