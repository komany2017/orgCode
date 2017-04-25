#ifndef QDOWNLOAD_H
#define QDOWNLOAD_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkProxy>
#include <QFile>
#include <QTimer>
#include <mymodel.h>
#include <QUrl>
// 字节转KB、MB、GB
QString size(qint64 bytes);
// 速度转KB/S、MB/S、GB/S
QString speed(double speed);
// 秒转*d *h *m *s
QString timeFormat(int seconds);

class QDownload : public QObject
{
  Q_OBJECT
public:
  explicit QDownload(MyModel* m, int nrow, QObject *parent = 0);
protected slots:
	void httpFinished();
	void onDownloadProgress(qint64 bytesSent,qint64 bytesTotal);
	void readyRead();//
	void upLoadError(QNetworkReply::NetworkError errorCode);
signals:
	void finish();
public:
    void StartRequest(QString str);
    bool getFileTotalSize(QString strUrl, int tryTimes);
private:
  QUrl url;
  QNetworkAccessManager qnam;
  QNetworkReply *reply;
  QFile *file;
  int httpGetId;
  bool httpRequestAborted;
  MyModel* model;
  int row;
  QMutex mutex;
  QTime downloadTime;
  int m_nTime;
};

#endif // QDOWNLOAD_H
