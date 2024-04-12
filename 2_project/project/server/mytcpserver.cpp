#include "mytcpserver.h"
#include <QDebug>
#include <QCoreApplication>
#include <QString>


MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent){
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    if (!mTcpServer->listen(QHostAddress::Any, 33333)){
        qDebug() << "server is not started";
    } else {
        server_status=1;
        qDebug() << "server is started";
    }

    Database::getInstance()->initializeDatabase();
}

MyTcpServer::~MyTcpServer()
{
    mTcpServer->close();
    server_status = 0;
    //DatabaseDestroyer Database::destroyer;
}

void MyTcpServer::slotNewConnection(){
    if (server_status == 1){
        QTcpSocket *tempSocket;
        tempSocket = mTcpServer->nextPendingConnection();

        tempSocket->write("Hello!!!\r\n");

        connect(tempSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
        connect(tempSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);
        TcpSocketList.push_front(tempSocket);
    }
}


void MyTcpServer::slotServerRead() {
    QTcpSocket* tempSocket = (QTcpSocket*)sender();
    QString request="";
    while (tempSocket->bytesAvailable() > 0) {
        request.append(tempSocket->readAll());
    }

    QByteArray response = parse(request)+ "\r\n";
    qDebug() << request;
    tempSocket->write(response);
}

void MyTcpServer::slotClientDisconnected(){
    QTcpSocket* tempSocket = (QTcpSocket*) sender();
    tempSocket->close();
}
