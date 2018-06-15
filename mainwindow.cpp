#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    load();
    init(false);
    addFrameForm = new AddFrame(this);
    addSlotForm = new AddSlot(this);
    msg = new QMessageBox(this);
    connect(addFrameForm, SIGNAL(accepted(QString, QString)), this, SLOT(slot_addFrameAccepted(QString, QString)));
    connect(addSlotForm, SIGNAL(accepted(QString, QString, QString, QString)), this, SLOT(slot_addSlotAccepted(QString, QString, QString, QString)));

}

void MainWindow::slot_addFrameAccepted(QString parentName, QString value)
{
    Frame *frame = new Frame(value, parentName);
    Frame *parent = findParent(parentName);
    frame->m_parent = parent;
    parent->addChild(frame);
    frameList.push_back(frame);
    init(true);
}

void MainWindow::slot_addSlotAccepted(QString parentName, QString slotName, QString slotType, QString slotValue)
{
    Slot *slot = new Slot(slotName, slotType, slotValue, parentName);
    Frame *parent = findParent(parentName);
    parent->addSlot(slot);
    slot->m_parent = parent;
    slotList.push_back(slot);
    init(true);
}

void MainWindow::load()
{
    frameList.clear();
    slotList.clear();

    frameFileDesc = new QFile("frames.txt");
    frameFileDesc->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

    slotsFileDesc = new QFile("slots.txt");
    slotsFileDesc->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

    QTextStream frameStream(frameFileDesc);
    QTextStream slotStream(slotsFileDesc);
    QStringList line;

    while(!frameStream.atEnd())
    {
        line = frameStream.readLine().split(":");
        if(line.size() <= 1)
            break;
        Frame *frame = new Frame(line.at(0), line.at(1));
        frameList.push_back(frame);
    }

    while(!slotStream.atEnd())
    {
        line = slotStream.readLine().split(":");
        if(line.size() <= 1)
            break;
        Slot *slot = new Slot(line.at(0), line.at(1), line.at(2), line.at(3));
        slotList.push_back(slot);
    }

    frameFileDesc->close();
    slotsFileDesc->close();
}

void MainWindow::saveModel()
{
    frameFileDesc->open(QIODevice::ReadWrite | QIODevice::Unbuffered | QIODevice::Truncate);
    slotsFileDesc->open(QIODevice::ReadWrite | QIODevice::Unbuffered | QIODevice::Truncate);
    QTextStream frameStream(frameFileDesc);
    QTextStream slotStream(slotsFileDesc);

    for(int i = 0; i < frameList.size(); i++)
    {
        frameStream << frameList.at(i)->m_name << ":" << frameList.at(i)->parentName << "\r\n";
    }

    for(int i = 0; i < slotList.size(); i++)
    {
        slotStream << slotList.at(i)->m_name << ":" << slotList.at(i)->m_type << ":" <<
                      slotList.at(i)->m_value << ":" << slotList.at(i)->parentName << "\r\n";
    }

    frameFileDesc->close();
    slotsFileDesc->close();
}

QTreeWidgetItem *MainWindow::initTree(Frame *frame)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    if(!frame->added)
    {
        item->setText(0, frame->m_name);

        if(frame->m_childs.size() != 0)
            for(int i = 0; i < frame->m_childs.size(); i++)
                item->addChild(initTree(frame->m_childs.at(i)));

        if(frame->m_slots.size() != 0)
            for(int i = 0; i < frame->m_slots.size(); i++)
                item->addChild(initTree(frame->m_slots.at(i)));

        frame->added = true;
        return item;
    }

    delete item;
    return nullptr;
}


QTreeWidgetItem *MainWindow::initTree(Slot *slot)
{
    QColor color("red");
    QTreeWidgetItem *item = new QTreeWidgetItem();
    if(!slot->added)
    {
        item->setText(0, slot->m_name);
        item->setTextColor(0, color);
        slot->added = true;
        return item;
    }

    delete item;
    return nullptr;
}


void MainWindow::init(bool reInit)
{
    ui->treeWidget->clear();
    if(reInit)
    {
        for(Frame *frame : frameList)
            frame->added = false;

        for(Slot *slot : slotList)
            slot->added = false;
    }
    else
    {
        load();
    }

    QString parentName;
    Frame *parent;
    for(int i = 0; i < frameList.size(); i++)
    {
        parentName = frameList.at(i)->parentName;
        parent = findParent(parentName);
        if(parent != nullptr)
        {
            parent->addChild(frameList.at(i));
            frameList.at(i)->m_parent = parent;
        }
    }

    for(int i = 0; i < slotList.size(); i++)
    {
        parentName = slotList.at(i)->parentName;
        parent = findParent(parentName);
        if(parent != nullptr)
        {
            parent->addSlot(slotList.at(i));
            slotList.at(i)->m_parent = parent;
        }
    }

    for(int i = 0; i < frameList.size(); i++)
    {
        ui->treeWidget->addTopLevelItem(initTree(frameList.at(i)));
    }
}

Slot *MainWindow::findSlot(QString name)
{
    for(int i = 0; i < slotList.size(); i++)
        if(slotList.at(i)->m_name == name)
            return  slotList.at(i);

    return nullptr;
}

Frame *MainWindow::findParent(QString name)
{
    for(int i = 0; i < frameList.size(); i++)
        if(frameList.at(i)->m_name == name)
            return frameList.at(i);
    return nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_saveSlot_clicked()
{
    Slot *slot = findSlot(ui->treeWidget->currentItem()->text(0));
    if(slot != nullptr)
    {
        slot->m_name = ui->lineEdit_slotName->text();
        slot->m_value = ui->lineEdit_slotValue->text();
        slot->m_type = ui->comboBox_slotType->currentText();
    }
    init(true);
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    Slot *slot = findSlot(item->text(0));
    if(slot != nullptr)
    {
        ui->lineEdit_slotName->setText(slot->m_name);
        ui->lineEdit_slotValue->setText(slot->m_value);
        ui->comboBox_slotType->setCurrentText(slot->m_type);
    }
}

void MainWindow::on_action_addFrame_triggered()
{
    addFrameForm->setParentFramesList(frameList);
    addFrameForm->show();
}

void MainWindow::on_action_addSlot_triggered()
{
    addSlotForm->setParentFramesList(frameList);
    addSlotForm->show();
}

bool MainWindow::deleteFrame(QString name)
{
    int i = 0;
    for(Frame *frame : frameList)
    {
        if(frame->m_name == name)
        {
            frameList.removeAt(i);

            return true;
        }
        i++;
    }
    return false;
}

bool MainWindow::deleteSlot(QString name)
{
    int i = 0;
    for(Slot *slot : slotList)
    {
        if(slot->m_name == name)
        {
            slotList.removeAt(i);
            slot->m_parent->deleteSlot(name);
            delete slot;
            return true;
        }
        i++;
    }
    return false;
}

void MainWindow::on_pushButton_clicked()
{
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    if(deleteFrame(item->text(0)) || deleteSlot(item->text(0)))
    {
        init(true);
    }
    else
    {
        qDebug() << "err";
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    saveModel();
}

void MainWindow::on_pushButton_3_clicked()
{
    load();
    init(false);
}

QString MainWindow::find(Frame *frame)
{
    QString temp;
    for(Frame *fr : frame->m_childs)
    {

        temp.push_back(fr->m_name + ", ");
        temp.push_back(find(fr));
    }

    return temp;
}

void MainWindow::on_pushButton_4_clicked()
{
    QString slotName = ui->lineEdit->text();
    QString value = ui->lineEdit_2->text();
    QString result;
    for(Slot *slot : slotList)
    {
        if(slot->m_name == slotName && slot->m_value == value)
        {
            result.push_back(slot->m_parent->m_name + ", ");
            for(Frame *frame : slot->m_parent->m_childs)
                result.push_back(find(frame));
        }

    }
    msg->setText(result);
    msg->show();
}
