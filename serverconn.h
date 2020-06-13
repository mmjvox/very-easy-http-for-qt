#ifndef SERVERCONN_H
#define SERVERCONN_H

#include <QObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QBuffer>
#include <QJsonObject>
#include <QHttpMultiPart>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDesktopServices>

class ServerConn : public QObject
{
    Q_OBJECT
public:
    explicit ServerConn(QObject *parent = nullptr);
    ~ServerConn();
    static ServerConn *instance;
    static ServerConn *inst();
    static ServerConn *SingletonProvider () {
      if(!ServerConnInstance){
          ServerConnInstance = new ServerConn();
      }
      return ServerConnInstance;
    }

    static QString serverHost;

    QNetworkAccessManager networkManager;


private:
    static ServerConn* ServerConnInstance;

signals:


public slots:

};
#endif // SERVERCONN_H
