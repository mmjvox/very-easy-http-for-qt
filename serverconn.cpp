#include "serverconn.h"

ServerConn *ServerConn::ServerConnInstance = nullptr;


QString ServerConn::serverHost="http://localhost";

ServerConn::ServerConn(QObject *parent) : QObject(parent)
{
//    networkManager= new QNetworkAccessManager;
}

ServerConn::~ServerConn(){}



