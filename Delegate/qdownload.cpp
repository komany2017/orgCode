#include "qdownload.h"
#include <QFileInfo>
#include <QMessageBox>
#include <qdir.h>
#include <time.h>
#include "qeventloop.h"
#include <QLineEdit>
#include <QLabel>
#define FILENAMECOL  0
#define TOTALSIZECOL 1
#define PROGRESSCOL  2
#define SPEEDCOL     3
#define FRESHTIMECOL 4

QDownload::QDownload(MyModel* m, int nrow, QObject *parent) : QObject(parent),model(m),row(nrow)
{
      file = NULL;
      row = nrow;
      model->appendRow(new QStandardItem());
      QModelIndex indexA = this->model->index(row, PROGRESSCOL);
      model->mymap[indexA] = 0.0;
      model->setData(indexA, QString::number(0));
      httpRequestAborted = false;
}
// 根据输入下载url，请求获取文件信息l
bool QDownload::getFileTotalSize(QString strUrl, int tryTimes)
{
    qint64 fileSize = -1;
    bool isGetResult = false;

    if (tryTimes <= 0)
    {
        tryTimes = 1;
    }

    do
    {

        QNetworkAccessManager manager;
        // 事件循环，等待请求文件头信息结束;
        QEventLoop loop;
        // 超时，结束事件循环;
        QTimer timer;

        //发出请求，获取文件地址的头部信息;
        QNetworkReply *reply1 = manager.head(QNetworkRequest(strUrl));
        if (!reply1)
            continue;

        connect(reply1, SIGNAL(finished()), &loop, SLOT(quit()));
        connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));

        timer.start(2000);
        loop.exec();

        if (reply1->error() != QNetworkReply::NoError)
        {
            // 请求发生错误;
            qDebug() << reply1->errorString();
            continue;
        }
        else if (!timer.isActive())
        {
            // 请求超时超时,未获取到文件信息;
            qDebug() << "Request Timeout";
            continue;
        }

        timer.stop();

        isGetResult = true;
        // 获取文件大小;
        QVariant var = reply1->header(QNetworkRequest::ContentLengthHeader);
        fileSize = var.toLongLong();
        // 获取文件名;
        // 先根据请求返回的信息中获取文件名，因为有些文件服务器未必会返回文件名信息(包括文件名，文件大小);可使用下面方法;
        // QFileInfo 会根据 url 返回 下载文件名;但也会失败，返回为空值;
        QString strDisposition = reply1->rawHeader("Content-Disposition");
        int index = strDisposition.indexOf("filename=");
        QString fileName = strDisposition.mid(index + 9);
        if (fileName.isEmpty())
        {
            QUrl url(strUrl);
            QFileInfo fileInfo(url.path());
            fileName += fileInfo.fileName();
        }

        //showFileInfo(fileName , transformUnit(fileSize));
        QModelIndex indexA = this->model->index(row, FILENAMECOL);
        model->setData(indexA, fileName);
        QModelIndex indexB = this->model->index(row , 1);
        mutex.lock();
        model->setData(indexB, size(fileSize));
        model->mymap[indexA] = fileSize;
        reply1->deleteLater();

        break;
    } while (tryTimes--);

    if (!isGetResult)
    {
        emit finish();
        QModelIndex indexs = this->model->index(row, SPEEDCOL);
        model->setData(indexs, "failed!" );
    }
    return isGetResult;
}

void QDownload::StartRequest(QString str)
{
   // if(!getFileTotalSize(str, 5))
        //return;
   
   QLineEdit* urlLineEdit = new QLineEdit(str);
   QLabel*    urlLabel = new QLabel(tr("&URL:"));
   urlLabel->setBuddy(urlLineEdit);
   url = urlLineEdit->text();
   delete urlLineEdit;
   delete urlLabel;
   QFileInfo fileInfo(url.path());
   QDir dir;
   QString path=dir.currentPath();

   QString fileName = fileInfo.fileName();

   QModelIndex indexA = this->model->index(row, FILENAMECOL);
   model->setData(indexA, fileName);

   if (fileName.isEmpty())
       fileName = "index.html";
   QString newpath = path + "/"+ "download";
   QString FilePath = fileInfo.filePath();
   model->mymapf[row] = newpath;

   dir.mkdir(newpath);
   file = new QFile(newpath + "/" + fileName);
   if (QFile::exists(fileName)) {
       QFile::remove(fileName);
   }
   if (!file->open(QIODevice::WriteOnly)) {
       delete file;
       file = 0;
       return;
     }

  //QModelIndex indexA = this->model->index(row, FILENAMECOL);
  //model->setData(indexA, fileName);
   // 如果有代理需要设置代理;
   //      QNetworkProxy proxy;
   //      proxy.setType(QNetworkProxy::HttpProxy);
   //      proxy.setHostName("proxy.example.com");
   //      proxy.setPort(1080);
   //      proxy.setUser("username");
   //      proxy.setPassword("password");
   //      QNetworkProxy::setApplicationProxy(proxy);

  reply = qnam.get(QNetworkRequest(url));
  connect(reply, SIGNAL(finished()),
          this, SLOT(httpFinished()));
  connect(reply, SIGNAL(readyRead()),
          this, SLOT(readyRead()));
  connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
          this, SLOT(onDownloadProgress(qint64,qint64)));
  connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(upLoadError(QNetworkReply::NetworkError)));
  downloadTime.start();
  m_nTime = 0;
}



void QDownload::httpFinished()
{

  if (httpRequestAborted) {
      if (file) {
          file->close();
          file->remove();
          delete file;
          file = 0;
      }
      reply->deleteLater();
      emit finish();
      QModelIndex indexs = this->model->index(row, SPEEDCOL);
      model->setData(indexs, "cancle" );
      return;
  }
  if(file) {
      file->flush();
      file->close();
    }

  QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
  if (!redirectionTarget.isNull())
  {
      QUrl newUrl = url.resolved(redirectionTarget.toUrl());

      url = newUrl;
      reply->deleteLater();
      file->open(QIODevice::WriteOnly);
      file->resize(0);
      StartRequest(url.url());
      return;
  }
  
  if(reply)
      reply->deleteLater();
  reply = 0;
  delete file;
  file = 0;

  emit finish();
  QModelIndex indexs = this->model->index(row, SPEEDCOL);
  model->setData(indexs, "finished" );
  QModelIndex indexT = this->model->index(row, FRESHTIMECOL);
  model->setData(indexT, "--:--") ;

}

QString Time2String( double dwTime)
{
    char cTime[50] = {0};
    memset(cTime,0,50);
    time_t t(dwTime);
    strftime(cTime,32,"%H:%M:%S",localtime(&t));
    return QString(cTime);
}

// 字节转KB、MB、GB
QString size(qint64 bytes)
{
    QString strUnit;
    double dSize = bytes * 1.0;
    if (dSize <= 0)
    {
        dSize = 0.0;
    }
    else if (dSize < 1024)
    {
        strUnit = "Bytes";
    }
    else if (dSize < 1024 * 1024)
    {
        dSize /= 1024;
        strUnit = "KB";
    }
    else if (dSize < 1024 * 1024 * 1024)
    {
        dSize /= (1024 * 1024);
        strUnit = "MB";
    }
    else
    {
        dSize /= (1024 * 1024 * 1024);
        strUnit = "GB";
    }

    return QString("%1 %2").arg(QString::number(dSize, 'f', 2)).arg(strUnit);
}

// 速度转KB/S、MB/S、GB/S
QString speed(double speed)
{
    QString strUnit;
    if (speed <= 0)
    {
        speed = 0;
        strUnit = "Bytes/S";
    }
    else if (speed < 1024)
    {
        strUnit = "Bytes/S";
    }
    else if (speed < 1024 * 1024)
    {
        speed /= 1024;
        strUnit = "KB/S";
    }
    else if (speed < 1024 * 1024 * 1024)
    {
        speed /= (1024 * 1024);
        strUnit = "MB/S";
    }
    else
    {
        speed /= (1024 * 1024 * 1024);
        strUnit = "GB/S";
    }

    QString strSpeed = QString::number(speed, 'f', 2);
    return QString("%1 %2").arg(strSpeed).arg(strUnit);
}

// 秒转*d *h *m *s
QString timeFormat(int seconds)
{
    QString strValue;
    QString strSpacing(" ");
    if (seconds <= 0)
    {
        strValue = QString("%1s").arg(0);
    }
    else if (seconds < 60)
    {
        strValue = QString("%1s").arg(seconds);
    }
    else if (seconds < 60 * 60)
    {
        int nMinute = seconds / 60;
        int nSecond = seconds - nMinute * 60;
        strValue = QString("%1m").arg(nMinute);
        if (nSecond > 0)
            strValue += strSpacing + QString("%1s").arg(nSecond);
    }
    else if (seconds < 60 * 60 * 24)
    {
        int nHour = seconds / (60 * 60);
        int nMinute = (seconds - nHour * 60 * 60) / 60;
        int nSecond = seconds - nHour * 60 * 60 - nMinute * 60;
        strValue = QString("%1h").arg(nHour);
        if (nMinute > 0)
            strValue += strSpacing + QString("%1m").arg(nMinute);
        if (nSecond > 0)
            strValue += strSpacing + QString("%1s").arg(nSecond);
    }
    else
    {
        int nDay = seconds / (60 * 60 * 24);
        int nHour = (seconds - nDay * 60 * 60 * 24) / (60 * 60);
        int nMinute = (seconds - nDay * 60 * 60 * 24 - nHour * 60 * 60) / 60;
        int nSecond = seconds - nDay * 60 * 60 * 24 - nHour * 60 * 60 - nMinute * 60;

        strValue = QString("%1d").arg(nDay);

        if (nHour > 0)
            strValue += strSpacing + QString("%1h").arg(nHour);

        if (nMinute > 0)
            strValue += strSpacing + QString("%1m").arg(nMinute);

        if (nSecond > 0)
            strValue += strSpacing + QString("%1s").arg(nSecond);
    }
    return strValue;
}

void QDownload::onDownloadProgress(qint64 bytesSent,qint64 bytesTotal)
{
  // 总时间
   int nTime = downloadTime.elapsed();
   // 本次下载所用时间
   nTime -= m_nTime;
   // 下载速度
   double dBytesSpeed = (bytesSent * 1000.0) / nTime;
   double dSpeed = dBytesSpeed;
   QModelIndex indexs = this->model->index(row, SPEEDCOL);
   model->setData(indexs, ((((speed(dSpeed) )))) );
  //剩余时间
  qint64 leftBytes = (bytesTotal - bytesSent);
  double dLeftTime = (leftBytes * 1.0) / dBytesSpeed;
  QModelIndex indexT = this->model->index(row, FRESHTIMECOL);
  model->setData(indexT, timeFormat(dLeftTime)) ;

  QModelIndex indexA = this->model->index(row, PROGRESSCOL);
  QModelIndex indexB = this->model->index(row , 1);
 // QStandardItem item( size(bytesTotal));
  mutex.lock();
  model->setData(indexB, size(bytesTotal));
  model->mymap[indexA] = bytesTotal;
  model->setData(indexA, QString::number((((bytesSent )))) );
  mutex.unlock();
  m_nTime = nTime;
}

void QDownload::readyRead()
{
  QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

  if(status_code == 200 && (reply->error() == QNetworkReply::NetworkError::NoError) && file && reply)
    {
      QByteArray b = reply->readAll();
        file->write(b);
    }
  else
  {
      QMessageBox::information(NULL, "error", reply->errorString(), QMessageBox::Yes, QMessageBox::Yes);
  }
}

void QDownload::upLoadError(QNetworkReply::NetworkError errorCode)
{
    errorCode = errorCode;
}
