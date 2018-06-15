#ifndef ADDSLOT_H
#define ADDSLOT_H

#include <QDialog>
#include <frame.h>
#include <slot.h>

namespace Ui {
class AddSlot;
}

class AddSlot : public QDialog
{
    Q_OBJECT

public:
    explicit AddSlot(QWidget *parent = 0);
    void setParentFramesList(QList <Frame*> frameList);
    ~AddSlot();

private slots:
    void on_buttonBox_accepted();


signals:

    void accepted(QString parentName, QString slotName, QString slotType, QString slotValue);

private:
    Ui::AddSlot *ui;
};

#endif // ADDSLOT_H
