#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileInfoList>
#include <QMainWindow>
#include <QDirModel>
#include <QFILE>
#include <QFileDialog>
#include <QtZlib/zlib.h>
#include <QDebug>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QThread>
#include <QuaZIP/inlcude/JlCompress.h>
#include <QSqlQueryModel>
#include <QTableView>
#include <QThread>

#define _AFXDLL
#include <QStandardItem>
#include <afx.h>
namespace Ui {
class MainWindow;
}

class ThreadRun : public QThread                   //标准线程
{
    Q_OBJECT
public:
    ThreadRun(QObject* parent = NULL) : QThread(parent) {}
signals:
    void sigBugsignal();
public:
    void run()
    {
        while(true) {
            qDebug() << "##########thread#########" << QThread::currentThreadId();
            qDebug("----------sig-------------");
            emit sigBugsignal();
            qDebug("--------sig-end----------");
            qDebug() << "#######################";
            usleep(2000000);
        }
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    bool DelDir(const QString &path) ;
    void DoFile();
private slots:
    void on_toolButton_clicked();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

protected:
    bool OpenDatabase();
    bool DishString(QString FileName,QString fn, QString s);
private:
    Ui::MainWindow *ui;
    QString       Path;
    QFileInfoList list;
    QSqlDatabase db;
    QStringList   FileList;
    QStandardItemModel *standardItemModel ;
};

#endif // MAINWINDOW_H
