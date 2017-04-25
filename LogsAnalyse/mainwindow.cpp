#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItem>
#include <QUrl>


inline QString  CS2QS(CString cs) { return  QString::fromWCharArray((LPCTSTR)cs, cs.GetLength()); }
inline CString  QS2CS(QString qs) { return  qs.toStdString().c_str(); }


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    OpenDatabase();
    CString str;
   // standardItemModel = new QStandardItemModel(this);
   // ui->listView->setModel(standardItemModel);
}

MainWindow::~MainWindow()
{
    if(db.isOpen()) db.close();
    delete ui;
}


bool MainWindow::OpenDatabase()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(QString("DRIVER={SQL SERVER};"
                               "SERVER=%1;"
                               "DATABASE=%2;"
                               "UID=%3;"
                               "PWD=%4;").arg("192.168.81.21")
                       .arg("pbb_log")
                       .arg("pbblogsa")
                       .arg("Admin@pbb.bj"));
    if (!db.open())
    {

        QMessageBox::warning(0, qApp->tr("Cannot open database"),
                             db.lastError().databaseText(), QMessageBox::Cancel);
        return false;
    }
    else
    {
        qDebug()<<"Connect to Database Success!";
        return true;
    }
    return true;

}


bool MainWindow::DelDir(const QString &path)
{
    if (path.isEmpty()){
        return false;
    }
    QDir dir(path);
    if(!dir.exists()){
        return true;
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo file, fileList)
    {
        if (file.isFile())
        {
            file.dir().remove(file.fileName());
        }
        else
        {
            DelDir(file.absoluteFilePath());
        }
    }
    return dir.rmpath(dir.absolutePath());
}


std::string GBK_To_UTF8(const std::string &source)
{
    enum {GB2312 = 936};

    unsigned long len = ::MultiByteToWideChar(GB2312, NULL, source.c_str(), -1, NULL, NULL);
    if (len == 0)
        return std::string();
    wchar_t *wide_char_buffer = new wchar_t[len];
    ::MultiByteToWideChar(GB2312, NULL, source.c_str(), -1, wide_char_buffer, len);

    len = ::WideCharToMultiByte(CP_UTF8, NULL, wide_char_buffer, -1, NULL, NULL, NULL, NULL);
    if (len == 0)
    {
        delete [] wide_char_buffer;
        return std::string();
    }
    char *multi_byte_buffer = new char[len];
    ::WideCharToMultiByte(CP_UTF8, NULL, wide_char_buffer, -1, multi_byte_buffer, len, NULL, NULL);

    std::string dest(multi_byte_buffer);
    delete [] wide_char_buffer;
    delete [] multi_byte_buffer;
    return dest;
}

std::string UTF8_To_GBK(const std::string &source)
{
    enum {GB2312 = 936};

    unsigned long len = ::MultiByteToWideChar(CP_UTF8, NULL, source.c_str(), -1, NULL, NULL);
    if (len == 0)
        return std::string();
    wchar_t *wide_char_buffer = new wchar_t[len];
    ::MultiByteToWideChar(CP_UTF8, NULL, source.c_str(), -1, wide_char_buffer, len);

    len = ::WideCharToMultiByte(GB2312, NULL, wide_char_buffer, -1, NULL, NULL, NULL, NULL);
    if (len == 0)
    {
        delete [] wide_char_buffer;
        return std::string();
    }
    char *multi_byte_buffer = new char[len];
    ::WideCharToMultiByte(GB2312, NULL, wide_char_buffer, -1, multi_byte_buffer, len, NULL, NULL);

    std::string dest(multi_byte_buffer);
    delete [] wide_char_buffer;
    delete [] multi_byte_buffer;
    return dest;
}

bool MainWindow::DishString(QString FileName,QString fn, QString s)
{
    char time[1024] = "";
    char log_pid[1024] = "";
    char log_type[1024] = "";

    QString date = fn;

    date.remove(0,4);
    date.remove(date.length() - 4, date.length());

    if(date.isEmpty()) return false;

    sscanf(s.toStdString().c_str(), "%[^ ]", time);
    s.remove(0, strlen(time) );

    if(s.isEmpty())return false;

    while(!s.isEmpty() && s.at(0) == ' '){
        s = s.remove(0,1);
    }

    sscanf(s.toStdString().c_str(), "%[^ ]", log_pid);
    s.remove(0, strlen(log_pid));
    if(s.isEmpty())return false;
    while(!s.isEmpty() && s.at(0) == ' '){
        s = s.remove(0,1);
    }

    sscanf(s.toStdString().c_str(), "%[^ ]", log_type);
    s.remove(0, strlen(log_type));
    if(s.isEmpty())return false;

    QString d(log_type);
    d.remove(']');
    d.remove('[');

    QSqlQuery query =  db.exec("INSERT INTO TB_Log_Client_Old (date, time, ip,log_type,log_pid,log_content) "
                               "VALUES (:date, :time, :ip,:log_type,:log_pid,:log_content)");

    query.prepare("INSERT INTO TB_Log_Client_Old (date, time, ip,log_type,log_pid,log_content) "
                  "VALUES (:date, :time, :ip,:log_type,:log_pid,:log_content)");

    query.bindValue(":ip", FileName);
    query.bindValue(":date", date);
    query.bindValue(":time", time);
    query.bindValue(":log_type", d);
    query.bindValue(":log_content", s) ;
    query.bindValue(":log_pid", log_pid);

    if(query.exec())
    {
        return true;
    }
    qDebug() << query.lastError().databaseText();
    return false;
}

void MainWindow::DoFile()
{
    try{
        if(!db.isOpen()) OpenDatabase();
        QFileDialog *fd = new QFileDialog(this);
        Path = fd->getExistingDirectory(this);
        ui->label->setText(Path);
        QDir dir(Path);
        if(!dir.exists())
        {
            return;
        }
        dir.setFilter(QDir::AllDirs | QDir::NoSymLinks);
        list = dir.entryInfoList();

        int file_count = list.count();
        if(file_count <= 0)
        {
            return;
        }

        QStringList string_list;
        for(int i=0; i < list.count();i++)
        {
            QRegExp rx2("(//d+)(//.)(//d+)(//.)(//d+)(//.)(//d +)");

            QFileInfo file_info = list.at(i);
            QString FileName = file_info.fileName();

            if(FileName == "." || FileName == "..")continue;
            if(file_info.isDir())
            {
                QDir dirChild(file_info.filePath());
                dirChild.cd(FileName);
                QStringList filters;
                filters << "*.rar" << "*.zip";
                dirChild.setNameFilters(filters);
                QFileInfoList list2 = dirChild.entryInfoList();
                if(list2.isEmpty())continue;

                for(int i = 0; i < list2.count(); i++)
                {
                    QFileInfo fi = list2.at(i);
                    QString fn = fi.fileName();

                    QStringList l1 = JlCompress::extractDir(fi.filePath());

                    for(int j = 0; j < l1.count(); j++)
                    {
                        QString filename = l1.at(j);
                        QString lowname = filename.toLower();
                        //2017-01-20
                        if(strstr(lowname.toLatin1().data(),".log"))
                        {
                            QByteArray ary = filename.toLatin1();
                            QTextCodec *codec = QTextCodec::codecForName("GBK");
                            QFile fp(filename);
                            int nTotal = 0;
                            int nDeal = 0 ;
                            if(fp.open(QIODevice::ReadOnly))
                            {

                                while(!fp.atEnd())
                                {
                                    nTotal++;
                                    QByteArray line = fp.readLine();

                                    QString s(codec->toUnicode(line));

                                    if(DishString(FileName, fn, s))
                                    {
                                        nDeal++;
                                    }
                                }
                                fp.close();
                            }

                            if(nDeal == nTotal)
                            {
                                DelDir(filename);
                            }
                            else
                            {
                               // QStandardItem *item = new QStandardItem(filename);

                               // this->standardItemModel->appendRow(item);
                            }
                        }
                        else
                        {
                            DelDir(filename);
                        }
                    }
                   // DelDir("logs");
                }
               // DelDir(file_info.dir().dirName());
            }
        }
        QTextCodec *codec = QTextCodec::codecForName("GB2312");
        QString str = qApp->tr("是否删除源文件夹:");
        str += Path;
        int nRet = QMessageBox::warning(0, qApp->tr("ok"), str, QMessageBox::Cancel|QMessageBox::Ok);

        if(nRet == QMessageBox::Ok)
        {
            DelDir(Path);
        }

    }
    catch(...){

    }
}

void MainWindow::on_toolButton_clicked()
{
    DoFile();
}

void MainWindow::on_pushButton_clicked()
{
    if(!db.isOpen()) OpenDatabase();
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("select * from TB_Log_Client_Old");
    QTableView *view = new QTableView;
    view->setModel(model);
    view->show();

}
//
void MainWindow::on_pushButton_2_clicked()
{
    if(!db.isOpen()) OpenDatabase();
    db.exec("TRUNCATE TABLE TB_Log_Client_Old");

}
