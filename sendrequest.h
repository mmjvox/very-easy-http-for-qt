#ifndef SENDREQUEST_H
#define SENDREQUEST_H

#include <QObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QBuffer>
#include <QJsonObject>
#include <QHttpMultiPart>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>
#include <QTimer>
#include <QTime>

#include <ServerConn/serverconn.h>

QT_BEGIN_NAMESPACE

class SendRequest : public QObject
{
    Q_OBJECT
public:

    enum ConnectType{
      form_data,
      JSON
    };


    explicit SendRequest(QObject *parent = nullptr);
    ~SendRequest();

    ServerConn *serverConn;

    void sendRequest(QJsonObject args, QString link);
    void sendRequest(QJsonObject args, QString link,int type);
    void uploadDone(QByteArray data);
    void netError();
    QNetworkReply *reply;
    QTimer *timeOute;
    int nextTime;
    QJsonObject tempArgs;
    QString tempLink;
    int tryCount;

signals:
     void setRespons(QByteArray data);
     void finished();
     void error();

public slots:
      void respons();
      void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
      void onDownloadProgress(qint64 bytesSent, qint64 bytesTotal);
      void timeOuted();
      void sendRequestAfterInterruption(QJsonObject args, QString link);
      void sendJsonRequestAfterInterruption(QJsonObject args, QString link);

};

#endif // SENDREQUEST_H
