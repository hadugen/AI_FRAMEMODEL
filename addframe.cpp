#include "addframe.h"
#include "ui_addframe.h"

AddFrame::AddFrame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFrame)
{
    ui->setupUi(this);
}

AddFrame::~AddFrame()
{
    delete ui;
}


void AddFrame::setParentFramesList(QList<Frame *> frameList)
{
    ui->comboBox_parentFrame->clear();
    for(Frame *frame : frameList)
    {
        ui->comboBox_parentFrame->addItem(frame->m_name);
    }
}

void AddFrame::on_buttonBox_accepted()
{
    emit accepted(ui->comboBox_parentFrame->currentText(), ui->lineEdit_frameValue->text());
}
