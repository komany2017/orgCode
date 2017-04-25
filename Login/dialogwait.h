#ifndef DIALOGWAIT_H
#define DIALOGWAIT_H

#include <QDialog>
#include <QLabel>
#include <QTimer>

namespace Ui {
class DialogWait;
}

class DialogWait : public QDialog
{
    Q_OBJECT
private:
    QTimer *updateTimer;    //实现图片切换的间隔时间
    int index;                //图片索引值
    QTimer *CloseTimer;    //实现图片切换的间隔时间
private slots:
    void updateLabel();        //槽函数实现更新图片
    void killdialog();
public:
    explicit DialogWait(QWidget *parent = 0);
    ~DialogWait();
private:
  Ui::DialogWait *ui;
  QLabel *l1;
  QLabel *l2;
};

#endif // DIALOGWAIT_H
