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

    QString m_name, m_type, m_value, parentName;
    Frame *m_parent;
    bool added;
signals:

public slots:
};

#endif // SLOT_H
