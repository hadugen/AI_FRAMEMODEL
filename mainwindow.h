#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <frame.h>
#include <QAbstractItemModel>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <addframe.h>
#include <addslot.h>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QList <Frame*> frameList;
    QList <Slot*> slotList;
    Slot *findSlot(QString name);
    void init(bool reInit);
    void load();
    QFile *slotsFileDesc, *frameFileDesc;
    QTreeWidgetItem *initTree(Frame *frame);
    QTreeWidgetItem *initTree(Slot *slot);
    Frame *findParent(QString name);
    bool deleteFrame(QString name);
    bool deleteSlot(QString name);
    QString find_childs(Frame *frame);

    void saveModel();

private slots:

    void on_pushButton_saveSlot_clicked();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_action_addFrame_triggered();

    void slot_addFrameAccepted(QString parentName, QString value);

    void slot_addSlotAccepted(QString parentName, QString slotName, QString slotType, QString slotValue);

    void on_action_addSlot_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    AddFrame *addFrameForm;
    AddSlot *addSlotForm;
    QMessageBox *msg;
};

#endif // MAINWINDOW_H
