#ifndef ADDFRAME_H
#define ADDFRAME_H

#include <QDialog>
#include <frame.h>
#include <slot.h>

namespace Ui {
class AddFrame;
}

class AddFrame : public QDialog
{
    Q_OBJECT

public:
    explicit AddFrame(QWidget *parent = 0);
    ~AddFrame();
    void setParentFramesList(QList <Frame*> frameList);

private slots:
    void on_buttonBox_accepted();

signals:
    void accepted(QString parentName, QString value);

private:
    Ui::AddFrame *ui;
};

#endif // ADDFRAME_H
