#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QStyledItemDelegate>
#include <QApplication>
#include <QStandardItemModel>
#include <QTableWidget>
#include <QTimer>
#include <QToolButton>
#include <mymodel.h>
#include <QThread>
#include <iostream>
#include <qdownload.h>
#include <QProcess>
#include <QDesktopServices>
#include <QMenu>

namespace Ui {
    class MainWindow;
}

class MyButton: public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit MyButton(QWidget *parent = 0) : QStyledItemDelegate(parent){

  }
  QWidget *createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
  {
    QPushButton* button = new QPushButton;
    button->setObjectName("click");
    return button;
  }
};

class TableViewDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TableViewDelegate(QWidget *parent = 0);
    ~TableViewDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index);
signals:
    void open(const QAbstractItemModel* model, const QModelIndex &index);
    void deleteData(const QAbstractItemModel* model, const QModelIndex &index);
private:
    QPoint m_mousePoint;  // 鼠标位置
    QScopedPointer<QPushButton> m_pOpenButton;
    QScopedPointer<QPushButton> m_pDeleteButton;
    QStringList m_list;
    int m_nSpacing;  // 按钮之间的间距
    int m_nWidth;  // 按钮宽度
    int m_nHeight;  // 按钮高度
    int m_nType;  // 按钮状态-1：划过 2：按下
    QMutex mutex;

};

class MyTabelView : public QTableView
{
    Q_OBJECT
public:
    MyTabelView(QWidget * parent = 0) : QTableView(parent)
    {

    }

};



typedef struct tagDownLoadFile
{
  QDownload* d;
  QString    url;
}DownLoadFile;



class Thread : public QThread
{
  Q_OBJECT
public:
  Thread(MyModel* model);
  void setMessage(QString message);
  void stop();
public slots:
  void finish()
  {
    stopped = true;
  }

protected:
  void run();
  void printMessage();

private:
  DownLoadFile* pDownLoad;
  QString messageStr;
  volatile bool stopped;
};

class ButtonContextMenu : public QToolButton
{
    Q_OBJECT

public:
    ButtonContextMenu(QWidget *parent = 0);
    ~ButtonContextMenu();

protected slots:
    void actionOneSlot();
    void actionTwoSlot();
    void actionThreeSlot();

private:
    QAction *m_actionOne;
    QAction *m_actionTwo;
    QAction *m_actionThree;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void open(const QAbstractItemModel* model, const QModelIndex &index)
    {
        const MyModel* m = static_cast<const MyModel*>(model);
        if(m)
          {
            QString path = m->mymapf[index.row()];
            QDesktopServices::openUrl(QUrl(path, QUrl::TolerantMode));
          }
    }

    void deleteData(const QAbstractItemModel* model, const QModelIndex &index)
    {

    }
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void DownLoad();
    virtual void mousePressEvent(QMouseEvent* e);    // 鼠标的单击事件
    virtual void mouseMoveEvent(QMouseEvent* e);    // 鼠标的移动事件
    virtual void mouseReleaseEvent(QMouseEvent* e);  // 鼠标的单击释放事件
public slots:
    void CloseWindow();
    void ShowMenu();
    void Small();
    void actionOneSlot();
    void actionTwoSlot();
private:
    Ui::MainWindow *ui;
    QTableView * table;
    QTimer t;
    QToolButton *closeButton;
    QToolButton *small;
    ButtonContextMenu *about;
    MyModel* model ;
    QList<DownLoadFile*> list;
    bool m_bIsWindowMoveable;
    QPoint pt;

};

#endif // MAINWINDOW_H
