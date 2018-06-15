#ifndef SLOT_H
#define SLOT_H

#include <QObject>
#include <frame.h>

class Frame;

class Slot : public QObject
{
    Q_OBJECT
public:
    explicit Slot(QString name, QString type, QString value, QString parentName, QObject *parent = nullptr);
    QString _name, _type, _value, _parentName;
    Frame *_parent;
    bool added;

signals:

public slots:

private:

};

#endif // SLOT_H
