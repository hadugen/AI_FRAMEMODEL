#include "addslot.h"
#include "ui_addslot.h"

AddSlot::AddSlot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddSlot)
{
    ui->setupUi(this);
}

AddSlot::~AddSlot()
{
    delete ui;
}


void AddSlot::setParentFramesList(QList<Frame *> frameList)
{
    ui->comboBox_parentFrame->clear();
    for(Frame *frame : frameList)
    {
        ui->comboBox_parentFrame->addItem(frame->_name);
    }

}
void AddSlot::on_buttonBox_accepted()
{
    emit accepted(ui->comboBox_parentFrame->currentText(), ui->lineEdit_slotName->text(),
                  ui->comboBox_slotType->currentText(), ui->lineEdit_slotValue->text());
}
