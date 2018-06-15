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
    frame->_parent = parent;
    parent->addChild(frame);
    frameList.push_back(frame);
    init(true);
}

void MainWindow::slot_addSlotAccepted(QString parentName, QString slotName, QString slotType, QString slotValue)
{
    Slot *slot = new Slot(slotName, slotType, slotValue, parentName);
    Frame *parent = findParent(parentName);
    parent->addSlot(slot);
    slot->_parent = parent;
    slotList.push_back(slot);
    init(true);
}

void MainWindow::load()             // функция загружает данные слотов и фреймов и записывает их в slotList и frameList соответственно
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

void MainWindow::saveModel()                // функция фиксирует изменение в фреймах и слотах (кнопка сохранить под деревом)
{
    frameFileDesc->open(QIODevice::ReadWrite | QIODevice::Unbuffered | QIODevice::Truncate);
    slotsFileDesc->open(QIODevice::ReadWrite | QIODevice::Unbuffered | QIODevice::Truncate);
    QTextStream frameStream(frameFileDesc);
    QTextStream slotStream(slotsFileDesc);

    for(Frame *frame : frameList)
    {
        frameStream << frame->_name << ":" << frame->_parentName << "\r\n";
    }

    for(Slot *slot : slotList)
    {
        slotStream << slot->_name << ":" <<slot->_type << ":" <<
                      slot->_value << ":" << slot->_parentName << "\r\n";
    }

    frameFileDesc->close();
    slotsFileDesc->close();
}

QTreeWidgetItem *MainWindow::initTree(Frame *frame)     // перегруженная рекурсивная функция инициализирующая дерево
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    if(!frame->added)
    {
        item->setText(0, frame->_name);

        for(Frame *childFrame : frame->_childs)
            item->addChild(initTree(childFrame));

        for(Slot * childSlot : frame->_slots)
            item->addChild(initTree(childSlot));

        frame->added = true; // пометка того, что мы прошли через этот узел
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
        item->setText(0, slot->_name);
        item->setTextColor(0, color);
        slot->added = true;
        return item;
    }

    delete item;
    return nullptr;
}


void MainWindow::init(bool reInit)          //функция собирает дерево с узлами в кучу и выводит.
{
    ui->treeWidget->clear();

    if(reInit)
    {
        for(Frame *frame : frameList)
            frame->added = false;
                                        // помечаем узлы как не посещённые
        for(Slot *slot : slotList)
            slot->added = false;
    }
    else
    {
        load();                         // загрузка из файла
    }

    QString parentName;
    Frame *parent;
    for(Frame *frame : frameList)           //записываем всех потомков фрейма в него
    {
        parentName = frame->_parentName;
        parent = findParent(parentName);
        if(parent != nullptr)
        {
            parent->addChild(frame);
            frame->_parent = parent;
        }
    }

    for(Slot *slot : slotList)              //записываем все слоты фрейма в него
    {
        parentName = slot->_parentName;
        parent = findParent(parentName);
        if(parent != nullptr)
        {
            parent->addSlot(slot);
            slot->_parent = parent;
        }
    }

    for(Frame *frame : frameList)
    {
        ui->treeWidget->addTopLevelItem(initTree(frame));   // выводим дерево
    }
}

Slot *MainWindow::findSlot(QString name)    //ищем слот по имени
{
    for(Slot *slot : slotList)
        if(slot->_name == name)
            return  slot;

    return nullptr;
}

Frame *MainWindow::findParent(QString name) //ищем потомка по имени
{
    for(Frame *frame : frameList)
        if(frame->_name == name)
            return frame;

    return nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_saveSlot_clicked()           //фиксируем и обновляем дерево
{
    Slot *slot = findSlot(ui->treeWidget->currentItem()->text(0));
    if(slot != nullptr)
    {
        slot->_name = ui->lineEdit_slotName->text();
        slot->_value = ui->lineEdit_slotValue->text();
        slot->_type = ui->comboBox_slotType->currentText();
    }
    init(true);
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)  //слот, который выводит значения слота (лол)
{
    Slot *slot = findSlot(item->text(0));
    if(slot != nullptr)
    {
        ui->lineEdit_slotName->setText(slot->_name);
        ui->lineEdit_slotValue->setText(slot->_value);
        ui->comboBox_slotType->setCurrentText(slot->_type);
    }
}

void MainWindow::on_action_addFrame_triggered()         //менюшка добавления фрейма нажата
{
    addFrameForm->setParentFramesList(frameList);
    addFrameForm->show();
}

void MainWindow::on_action_addSlot_triggered()         //менюшка добавления ckjnf нажата
{
    addSlotForm->setParentFramesList(frameList);
    addSlotForm->show();
}

bool MainWindow::deleteFrame(QString name)              // удалаем фрейм по имени
{
    int i = 0;
    for(Frame *frame : frameList)
    {
        if(frame->_name == name)
        {
            frameList.removeAt(i);

            return true;
        }
        i++;
    }
    return false;
}

bool MainWindow::deleteSlot(QString name)           // удалаем слот по имени
{
    int i = 0;
    for(Slot *slot : slotList)
    {
        if(slot->_name == name)
        {
            slotList.removeAt(i);
            slot->_parent->deleteSlot(name);
            delete slot;
            return true;
        }
        i++;
    }
    return false;
}

void MainWindow::on_pushButton_clicked()                            // нажата кнопка удаления слота/фрейма
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

void MainWindow::on_pushButton_2_clicked()          //нажата кнопка сохранения
{
    saveModel();
}

void MainWindow::on_pushButton_3_clicked()          // нажата кнопка обновления
{
    load();
    init(false);
}

QString MainWindow::find_childs(Frame *frame)
{
    QString temp;
    for(Frame *fr : frame->_childs)
    {

        temp.push_back(fr->_name + ", ");
        temp.push_back(find_childs(fr));
    }

    return temp;
}

void MainWindow::on_pushButton_4_clicked()          //кнопка поиска нажата
{
    QString slotName = ui->lineEdit->text();
    QString value = ui->lineEdit_2->text();
    QString result;
    for(Slot *slot : slotList)
    {
        if(slot->_name == slotName && slot->_value == value)    //если найден нужный слот и нужное значение
        {
            result.push_back(slot->_parent->_name + ", ");      // добавляем его в результат
            for(Frame *frame : slot->_parent->_childs)          // добавляем в результат всех его потомков
                result.push_back(find_childs(frame));           // и потомков потомков
        }

    }
    msg->setText(result);
    msg->show();
}
