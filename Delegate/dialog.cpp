#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Dialog)
{
  ui->setupUi(this);
  ui->label->setPixmap(QPixmap(":/gif/images/1"));//初始化为第一张图片
  index = 1;//初始化索引值
  setWindowFlags(Qt::FramelessWindowHint);
  setWindowOpacity(0.9);

  setAttribute(Qt::WA_TranslucentBackground);
  updateTimer = new QTimer(this);//创建定时器
  updateTimer->setInterval(50);//设置定时器间隔50ms
  connect(updateTimer, SIGNAL(timeout()),this,SLOT(updateLabel()));//连接信号和槽
  updateTimer->start();//开启定时器
 
}

Dialog::~Dialog()
{
  updateTimer->stop();
  delete updateTimer;
  delete ui;
}

void Dialog::updateLabel()
{
      index++;        //自增1
      if(index > 8)    //更具图片总数
      {
          index = 1;
      }
      QPixmap pixmap(":/gif/image/" + QString::number(index, 10));
      ui->label->setPixmap(pixmap);//更新图片
}
