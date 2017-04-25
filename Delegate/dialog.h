#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>
#include <QStackedWidget>
#include <QLabel>
namespace Ui {
  class Dialog;
}

class Dialog : public QDialog
{
  Q_OBJECT

public:
  explicit Dialog(QWidget *parent = 0);
  ~Dialog();
private:
    QTimer *updateTimer;    //实现图片切换的间隔时间
    int index;                //图片索引值
private slots:
    void updateLabel();        //槽函数实现更新图片
private:
  Ui::Dialog *ui;
  QLabel *l1;
  QLabel *l2;
  QStackedWidget* StackWidget;
};

#endif // DIALOG_H
