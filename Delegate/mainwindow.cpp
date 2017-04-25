#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qtooltip.h>
#include <QMouseEvent>
#include <QPushButton>
#include <QPainter>
#include <QProgressBar>
#include <QDebug>
#include "QQueue"

#define FILE_OPERATE_COLUMN 5
#define buttonsize 31

TableViewDelegate::TableViewDelegate(QWidget *parent)
    : QStyledItemDelegate(parent),
      m_pOpenButton(new QPushButton()),
      m_pDeleteButton(new QPushButton()),
      m_nSpacing(5),
      m_nWidth(25),
      m_nHeight(20)
{
    m_pOpenButton.data()->setObjectName("OpenButton");
}

TableViewDelegate::~TableViewDelegate()
{
}

// 绘制按钮
void TableViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
    QStyledItemDelegate::paint(painter, viewOption, index);
    int Column = index.column();
    const MyModel* m = (const MyModel*)(index.model());
    switch(Column)
    {
    case 0:
    {
        // QString name = index.data().toString();
        //QApplication::style()->drawItemText ( painter, option.rect , option.displayAlignment, QApplication::palette(), true,name);
        break;
    }
    case 1:
    {
        //QApplication::style()->drawItemText ( painter, option.rect , option.displayAlignment, QApplication::palette(), true, QString::number( qint64(m->totalByte / 1024) ) + " MB" );
        break;
    }
    case 2:
    {
        if(m->mymap[index] != 0.0)
        {
            QStyleOptionProgressBar progressBarOption;
            progressBarOption.initFrom(option.widget);
            progressBarOption.textAlignment = Qt::AlignCenter;
            progressBarOption.rect = option.rect.adjusted(4, 4, -4, -4);
            progressBarOption.minimum = 0;
            progressBarOption.maximum = 100;
            progressBarOption.textAlignment = Qt::AlignRight;
            progressBarOption.textVisible = true;
            progressBarOption.progress = (index.data().toDouble() / m->mymap[index] ) * 100;
            qint64 q = m->mymap[index];
            progressBarOption.text = size(index.data().toDouble()) + "/" + size(m->mymap[index] );
            QProgressBar bar;
            bar.setObjectName("bar");
            QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter,&bar);
        }
        break;
    }
    case FILE_OPERATE_COLUMN:
    {
        // 计算按钮显示区域
        int nTop = (option.rect.height() - m_nHeight) / 2;
        QStyleOptionButton button;
        button.iconSize = QSize(16,16);
        button.rect = QRect(option.rect.left() + (option.rect.width() - m_nWidth) / 2,option.rect.top() + nTop,  m_nWidth, m_nHeight);
        if (button.rect.contains(m_mousePoint))
        {
            if (m_nType == 0)
            {
                button.state |= QStyle::State_MouseOver;
            }
            else if (m_nType == 1)
            {
                button.state |= QStyle::State_Sunken;
            }
        }
        QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter,m_pOpenButton.data());
        break;
    }
    default:
        break;
    }
}


bool TableViewDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    if (index.column() == 0)
    {
        QMouseEvent *pEvent = static_cast<QMouseEvent*> (event);
        m_mousePoint = pEvent->pos();
        // 还原鼠标样式
        QApplication::restoreOverrideCursor();
        // 鼠标位于按钮之上
        if (option.rect.contains(m_mousePoint))
            QToolTip::showText(pEvent->globalPos(), index.data().toString());
    }
    if (index.column() == 5)
    {
        QMouseEvent *pEvent = static_cast<QMouseEvent*> (event);
        m_mousePoint = pEvent->pos();
        // 还原鼠标样式
        QApplication::restoreOverrideCursor();
        // 鼠标位于按钮之上
        if (option.rect.contains(m_mousePoint))
            QToolTip::showText(pEvent->globalPos(), "open file path");
    }

    if (index.column() == FILE_OPERATE_COLUMN)
    {
        m_nType = -1;
        bool bRepaint = false;
        QMouseEvent *pEvent = static_cast<QMouseEvent*> (event);
        m_mousePoint = pEvent->pos();
        // 还原鼠标样式
        QApplication::restoreOverrideCursor();
        int nTop = (option.rect.height() - m_nHeight) / 2;
        QStyleOptionButton button;
        button.rect = QRect(option.rect.left() + (option.rect.width() - m_nWidth) / 2,option.rect.top() + nTop,  m_nWidth, m_nHeight);
        button.text = index.data().toString();
        // 鼠标位于按钮之上
        if (!button.rect.contains(m_mousePoint))
            return false;
        bRepaint = true;
        switch (event->type())
        {
        // 鼠标滑过
        case QEvent::MouseMove:
        {
            m_nType = 0;
            break;
        }
            // 鼠标按下
        case QEvent::MouseButtonPress:
        {
            m_nType = 1;
            break;
        }
            // 鼠标释放
        case QEvent::MouseButtonRelease:
        {
            qDebug() << "click";
            emit open(model,index);
            break;
        }
        default:
            break;
        }
        if (m_nType == 0)
        {
            button.state |= QStyle::State_MouseOver;
        }
        else if (m_nType == 1)
        {
            button.state |= QStyle::State_Sunken;
        }
        return bRepaint;
    }
    return false;
}

ButtonContextMenu::ButtonContextMenu(QWidget *parent)
    : QToolButton(parent)
{
    setContextMenuPolicy(Qt::ActionsContextMenu);

    m_actionOne = new QAction(this);
    m_actionTwo = new QAction(this);
    m_actionThree = new QAction(this);

    m_actionOne->setText("ActionOne");
    m_actionTwo->setText("ActionTwo");
    m_actionThree->setText("ActionThree");

    addAction(m_actionOne);
    addAction(m_actionTwo);
    addAction(m_actionThree);

    connect(m_actionOne, SIGNAL(triggered()), this, SLOT(actionOneSlot()));
    connect(m_actionTwo, SIGNAL(triggered()), this, SLOT(actionTwoSlot()));
    connect(m_actionThree, SIGNAL(triggered()), this, SLOT(actionThreeSlot()));
}

ButtonContextMenu::~ButtonContextMenu()
{

}

void ButtonContextMenu::actionOneSlot()
{
    qDebug() << "actionOne";
}

void ButtonContextMenu::actionTwoSlot()
{
    qDebug() << "actionTwo";
}

void ButtonContextMenu::actionThreeSlot()
{
    qDebug() << "actionThree";
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_bIsWindowMoveable = false;
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowOpacity(0.9);
    t.setInterval(500);
    setAttribute(Qt::WA_TranslucentBackground);
    model = new MyModel(0,6);
    ui->tableView->setParent(ui->frame_2);
    ui->frame->setParent(ui->frame_2);
    table = ui->tableView;//new MyTabelView(this);
    model->setHeaderData(0,Qt::Horizontal,(("FileName")));
    model->setHeaderData(1,Qt::Horizontal,(("Size")));
    model->setHeaderData(2,Qt::Horizontal,(("Progress")));
    model->setHeaderData(3,Qt::Horizontal,(("speed")));
    model->setHeaderData(4,Qt::Horizontal,(("totaltime")));
    model->setHeaderData(5,Qt::Horizontal,(("Status")));
    //table->setShowGrid(false);
    table->setModel(model);
    table->horizontalHeader()->setVisible(false);// 水平不可见
    table->verticalHeader()->setVisible(false);// 垂直不可见
    TableViewDelegate* delegate = new TableViewDelegate;
    table->setItemDelegate(delegate);
    connect(delegate, SIGNAL(open(const QAbstractItemModel*,QModelIndex)), this, SLOT(open(const QAbstractItemModel*,QModelIndex)));

    //table->setItemDelegateForColumn(5, new MyButton);
    table->horizontalHeader()->resizeSection(1,80); //设置表头第一列的宽度为150
    table->horizontalHeader()->resizeSection(2,150); //设置表头第一列的宽度为150
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode ( QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    int nWidth = 0;
    for(int i = 0; i < 5; i++)
    {
        nWidth += table->horizontalHeader()->sectionSize(i);
    }

    table->horizontalHeader()->resizeSection(5,width() - nWidth);
    resize( table->width(), ui->frame->height() + table->height() + 30 );
    ui->frame_2->resize(width(), height());
    ui->frame->resize(table->width(), 34);
    table->resize(width(), table->height());
    small= new QToolButton(ui->frame);
    small->setObjectName("small");
    about= new ButtonContextMenu(ui->frame);
    about->setObjectName("about");
    closeButton= new QToolButton(ui->frame);
    closeButton->setObjectName("close");
    closeButton->setGeometry( ui->frame->width() - buttonsize,0,buttonsize,buttonsize);
    small->setGeometry(ui->frame->width()- 2 * buttonsize,0, buttonsize,buttonsize);
    about->setGeometry(ui->frame->width() - 3 * buttonsize,0, buttonsize,buttonsize);

    connect(small, SIGNAL(clicked(bool)), this, SLOT(Small()));
    connect(closeButton, SIGNAL(clicked(bool)), this, SLOT(CloseWindow()));
    connect(about, SIGNAL(clicked(bool)), this, SLOT(ShowMenu()));
    DownLoad();
}
 void MainWindow::actionOneSlot()
 {
 }
    void MainWindow::actionTwoSlot()
    {

    }

void MainWindow::CloseWindow()
{
    this->close();
}

void MainWindow::ShowMenu()
{

}

void MainWindow::Small()
{
   this->showMinimized();
}


MainWindow::~MainWindow()
{
    delete ui;
}


QQueue<DownLoadFile*> queueList;
QMutex g_mutex;
Thread::Thread(MyModel* model)
{
    g_mutex.lock();
    while (!queueList.isEmpty())
    {
        DownLoadFile *d1 = queueList.first();
        d1->d->StartRequest(d1->url);
        queueList.pop_front();
    }

    g_mutex.unlock();
    stopped = false;
}

void Thread::run()
{
    while(!stopped){

    }

    stopped = false;
}

void Thread::stop()
{
    stopped = true;
}


void Thread::setMessage(QString message)
{
    messageStr = message;
}

void Thread::printMessage()
{
    qDebug()<<messageStr;
    sleep(1);
}


void MainWindow::DownLoad()
{
    DownLoadFile *d1 = new DownLoadFile();
    d1->d = new QDownload(model, model->rowCount());
    d1->url = QString("https://qd.myapp.com/myapp/qqteam/tim/down/TIM1.0.20160.0.exe");
    d1->d->StartRequest(d1->url);
}


void MainWindow::mousePressEvent(QMouseEvent* e)
{
    if(e->button() ==  Qt::LeftButton)
    {
        m_bIsWindowMoveable = true;
        pt = e->pos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* e)
{

    if(m_bIsWindowMoveable)
    {

        move(e->globalPos() - pt);

    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent* e)
{
    m_bIsWindowMoveable = false;
}
