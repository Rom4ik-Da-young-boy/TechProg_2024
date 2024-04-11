#include <iostream>
#include "serverfunction.h"
#include "database.h"
#include "task1.h"
#include "task2.h"
#include "task.h"

QByteArray parse(QString request) {
    qDebug() << "\n";
    request = removeLastTwoCharacters(request);
    qDebug() << "\n";

    // Разбиение строки запроса на параметры
    QStringList data_client = request.split(QLatin1Char('&'));
    QString method = data_client.front();
    data_client.pop_front();
    QString response;

    if (method == QString("auth")) {
        response = auth(data_client.at(0), data_client.at(1));
    }
    else if (method == QString("reg")) {
        response = reg(data_client.at(0), data_client.at(1), data_client.at(2));
    }
    else if (method == QString("getUserStat")) {
        response = getUserStat(data_client.at(0));
    }
    else if (method == QString("task1")) {
        QList<int> list;
        for (const QString& str : data_client) {
            list.append(str.toInt());
        }
        response = executeTask1(list);
    }
    else if (method == QString("task2")) {
        QMap<int, std::pair<int, int>> vertexCoordinates;
        for (const QString& str : data_client) {
            QStringList coordinates = str.split(QLatin1Char(' '));
            int vertex = coordinates.at(0).toInt();
            int x = coordinates.at(1).toInt();
            int y = coordinates.at(2).toInt();
            vertexCoordinates[vertex] = std::make_pair(x, y);
        }
        response = executeTask2(vertexCoordinates.size(), vertexCoordinates);
    }
    else {
        // Сообщение об ошибке, если функция не определена
        response = "Error: Function is not defined";
    }
    return QByteArray((response + "\r\n").toUtf8());
}

QByteArray auth(QString login, QString input_password) {
    QVariantMap response;
    QString query = QString("SELECT * FROM Users WHERE login = \"%1\"").arg(login);
    QSqlQuery queryResult = Database::getInstance()->doSQLQuery(query);
    QSqlRecord record = queryResult.record();
    queryResult.first();
    QVariant dbPassword = queryResult.value(record.indexOf("password"));
    if (dbPassword == QVariant::Invalid) {
        response["status"] = "fail 1";
    }
    else if (dbPassword.toString() == input_password) {
        QString token = queryResult.value(record.indexOf("token")).toString();
        response["status"] = "success";
        response["token"] = token;
    }
    else {
        response["status"] = "fail1 2";
    }

    qDebug() << QString("Autherisation with login %1 and password %2").arg(login, input_password);

    // Сборка ответа в виде строки
    QString responseString;
    for (auto it = response.constBegin(); it!= response.constEnd(); ++it) {               //ПЕРЕДЕЛАТЬ!
        responseString += it.key() + ": " + it.value().toString() + "\n";
    }
    return responseString.toUtf8();
}

QByteArray reg(QString login, QString password, QString email) {
    QVariantMap response;
    QString token = QString(QCryptographicHash::hash(((login + password).toUtf8()), QCryptographicHash::Sha512).toHex());
    QString query = QString("INSERT INTO Users(login, password, email, token) "
                            "VALUES (\"%1\", \"%2\", \"%3\", \"%4\")")
                        .arg(login, password, email, token);
    Database::getInstance()->doSQLQuery(query);
    response["token"] = token;
    response["status"] = "success";

    // Сборка ответа в виде строки
    QString responseString;
    for (auto it = response.constBegin(); it!= response.constEnd(); ++it) {              //REDO
        responseString += it.key() + ": " + it.value().toString() + "\n";
    }
    return responseString.toUtf8();
}

QByteArray getUserStat(QString login) {
    QVariantMap response;
    QString query = QString("SELECT Tasks.type, COUNT(*) AS solved_tasks "
                            "FROM UsersTasks "
                            "JOIN Tasks ON Tasks.taskID = UsersTasks.taskID "
                            "WHERE UsersTasks.userID = (SELECT userID FROM Users WHERE login = \"%1\") "
                            "GROUP BY Tasks.type")
                        .arg(login);
    QSqlQuery stat = Database::getInstance()->doSQLQuery(query);
    QString responseString;
    while (stat.next()) {
        QString type = stat.value(0).toString();
        QString count = stat.value(1).toString();
        responseString += "Type: " + type + ", Solved tasks: " + count + "\n";
    }
    return responseString.toUtf8();
}

QByteArray executeTask1(const QList<int>& list) {
    // Выполняем задачу
    QList<int> result = divideListInHalf(list);

    // Формируем ответ
    QByteArray response = "Задание завершилось. Результат: ";
    for (int i : result) {
        response += QByteArray::number(i) + " ";
    }
    return response;
}

/*QByteArray executeTask2(int numVertices, const QMap<int, std::pair<int, int>>& vertexCoordinates) {
    // Создаем граф
    Graph graph(numVertices);

    // Добавляем ребра в граф
    for (int i = 1; i <= numVertices; ++i) {
        for (int j = i + 1; j <= numVertices; ++j) {
            int weight = std::sqrt(std::pow(vertexCoordinates[i].first - vertexCoordinates[j].first, 2) +
                                   std::pow(vertexCoordinates[i].second - vertexCoordinates[j].second, 2));
            graph.addEdge(i, j, weight);
        }
    }

    // Выполняем задачу
    QMap<int, int> distances = graph.shortestPath(1);

    // Формируем ответ
    QByteArray response = "Задание завершилось. Кратчайшее расстояние:";
    for (auto it = distances.begin(); it != distances.end(); ++it) {
        response += "\nVertex " + QByteArray::number(it.key()) + ": " + QByteArray::number(it.value());
    }
    return response;
}*/



QString removeLastTwoCharacters(QString str) {
    return str.left(str.length() - 2);
}
