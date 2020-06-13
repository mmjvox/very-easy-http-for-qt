#include "sendrequest.h"



SendRequest::SendRequest(QObject *parent) : QObject(parent)
{
    qDebug()<<__func__;
    serverConn = ServerConn::SingletonProvider();

    timeOute=new QTimer();
    connect(timeOute,&QTimer::timeout,this,&SendRequest::timeOuted);
    timeOute->setSingleShot(true);
    nextTime=20000;
    tryCount=1;

}

SendRequest::~SendRequest()
{
    qDebug()<<__func__<<tempLink<<"killed";
}

void SendRequest::sendRequest(QJsonObject args, QString link)
{
    QMetaObject::invokeMethod( this, "sendRequestAfterInterruption",Qt::QueuedConnection, Q_ARG( QJsonObject, args ), Q_ARG( QString, link ));
}


void SendRequest::sendRequest(QJsonObject args, QString link,int type)
{

    switch (type) {
        case 0:
            QMetaObject::invokeMethod( this, "sendRequestAfterInterruption",Qt::QueuedConnection, Q_ARG( QJsonObject, args ), Q_ARG( QString, link ));
        break;
        case 1:
            QMetaObject::invokeMethod( this, "sendJsonRequestAfterInterruption", Qt::QueuedConnection, Q_ARG( QJsonObject, args ) , Q_ARG( QString, link ));
        break;

    default:
        break;
    }
}

void SendRequest::sendRequestAfterInterruption(QJsonObject args, QString link)
{
    tempArgs=args;
    tempLink=link;
    qDebug()<<__func__<<link;

    timeOute->start(nextTime);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QJsonDocument doc(args);

    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"data\""));
    textPart.setBody(doc.toJson(QJsonDocument::Compact));
    multiPart->append(textPart);

        QUrl url(link);
        QNetworkRequest request(url);

        reply = serverConn->networkManager.post(request, multiPart);
        multiPart->setParent(reply);

//        connect(reply, &QNetworkReply::uploadProgress, [=](){
////            qInfo()<<"... ";
//        });

        connect(reply, &QNetworkReply::finished, this, &SendRequest::respons);

        connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            [=](QNetworkReply::NetworkError code){ netError(); });

        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit );
        connect(reply, &QNetworkReply::downloadProgress, this, &SendRequest::onDownloadProgress );
        loop.exec();

        multiPart->deleteLater();

}

void SendRequest::sendJsonRequestAfterInterruption(QJsonObject args, QString link)
{
    tempArgs=args;
    tempLink=link;
    qDebug()<<__func__<<link;

//    timeOute->start(nextTime);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QJsonDocument doc(args);
    QByteArray data = doc.toJson();

        QUrl url(link);
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        reply = serverConn->networkManager.post(request, data);
        multiPart->setParent(reply);

//        connect(reply, &QNetworkReply::uploadProgress, [=](){
////            qInfo()<<"... ";
//        });

        connect(reply, &QNetworkReply::finished, this, &SendRequest::respons);

        connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            [=](QNetworkReply::NetworkError code){ netError(); });

        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit );
        connect(reply, &QNetworkReply::downloadProgress, this, &SendRequest::onDownloadProgress );
        loop.exec();

        multiPart->deleteLater();
}

void SendRequest::respons(){
//    timeOute->stop();
    emit setRespons(reply->readAll());
//    emit finished();
    delete this;
}

void SendRequest::netError(){
    emit error();
    delete this;
}

void SendRequest::uploadDone(QByteArray data){
}

void SendRequest::uploadProgress(qint64 bytesSent, qint64 bytesTotal){

    qInfo()<<bytesSent/bytesTotal<<"   "<<bytesSent%bytesTotal;
}

void SendRequest::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    nextTime=20000;
    timeOute->start(nextTime);
    qDebug() << "Download progress:" << bytesReceived << bytesTotal;
}

void SendRequest::timeOuted()
{
    qDebug()<<__func__<<nextTime<<tryCount<<tempLink;
    disconnect(reply, &QNetworkReply::finished, this, &SendRequest::respons);
    disconnect(reply, &QNetworkReply::downloadProgress, this, &SendRequest::onDownloadProgress );

////    loop.exit();
//    reply->abort();

    if(tryCount<11){
        tryCount++;
        nextTime=40000+10000;
        sendRequest(tempArgs, tempLink);
    } else {
        tryCount=1;
        nextTime=40000;
//        netError();
    }
    timeOute->start(nextTime);
}
