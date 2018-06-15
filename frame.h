#ifndef FRAME_H
#define FRAME_H
#include <QObject>
#include <slot.h>
#include <QDebug>
class Slot;

class Frame : public QObject
{
    Q_OBJECT

private:



public:
    explicit Frame(QString name, QString parentName, QObject *parent = nullptr);
    void addChild(Frame *child);
    void deleteChild(QString name);
    void addSlot(Slot *slot);
    void deleteSlot(QString name);
    void setM_Parent(Frame *myParent);
    QString m_name, parentName;
    QList <Frame*> m_childs;
    QList <Slot*> m_slots;
    Frame* m_parent;
    QList <QChar> m_attr;
    bool added;


signals:

public slots:
};

#endif // FRAME_H
