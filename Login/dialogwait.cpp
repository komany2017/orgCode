#include "dialogwait.h"
#include "ui_dialogwait.h"

DialogWait::DialogWait(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWait)
{
    ui->setupUi(this);
    this->setObjectName("DialogWait");
    index = 1;//初始化索引值
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowOpacity(0.7);
    ui->label->setPixmap(QPixmap("images/Logon/1"));//初始化为第一张图片
    //setAttribute(Qt::WA_TranslucentB- ackground);
    updateTimer = new QTimer(this);//创建定时器
    CloseTimer = new QTimer(this);
    CloseTimer->setInterval(1500);
    updateTimer->setInterval(50);//设置定时器间隔50ms
    connect(updateTimer, SIGNAL(timeout()),this,SLOT(updateLabel()));//连接信号和槽
    connect(CloseTimer, SIGNAL(timeout()),this,SLOT(accept()));//连接信号和槽
    CloseTimer->start();
    updateTimer->start();//开启定时器

}

DialogWait::~DialogWait()
{
    updateTimer->stop();
    delete updateTimer;
    delete ui;
}

void DialogWait::updateLabel()
{
      index++;        //自增1
      if(index > 8)    //更具图片总数
      {
          index = 1;
      }
      QPixmap pixmap("image/Logon/" + QString::number(index, 10));
      ui->label->setPixmap(pixmap);//更新图片
}

void DialogWait::killdialog()
{}
