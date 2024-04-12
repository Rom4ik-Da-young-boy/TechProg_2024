#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject> //оперирует объектами
#include <QTcpServer>
#include <QTcpSocket> //оперируетсокетами

#include <QtNetwork> //сеть
#include <QByteArray> //побайтовая передача данных
#include <QDebug> //вывод в консоль сообщений
#include "serverfunction.h"

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();
public slots:
    void slotNewConnection();
    void slotClientDisconnected();
    void slotServerRead();

private:
    QTcpServer * mTcpServer;
    std::list<QTcpSocket*> TcpSocketList;
    int server_status;
};


#endif // MYTCPSERVER_H
