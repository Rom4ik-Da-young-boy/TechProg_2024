#ifndef serverfunction_h
#define serverfunction_h

#include <QCoreApplication>
#include <QString>
#include <stdio.h>
#include "database.h"
//#include "task.h"
#include <QDebug>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QList>
#include <QLatin1Char>
#include <QCryptographicHash>


QByteArray parse(QString request);
QByteArray auth(const QString& login, const QString& input_password);
QByteArray reg(const QString& login, const QString& password);
QByteArray checkTask(QString taskId, QString variant, QString answer);
QByteArray getTask(QString);
QByteArray getUserStat(QString login);
QString removeLastTwoCharacters(QString request);
QByteArray executeTask1();
QByteArray executeTask2();

#endif // SERVERFUNCTION_H
