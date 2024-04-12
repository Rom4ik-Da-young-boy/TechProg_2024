#include <iostream>
#include "serverfunction.h"
#include "database.h"
#include "task1.h"
#include "task2.h"
#include "task.h"

QString generateToken(int length = 32) {
    QString token;
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    const int possibleCharactersLength = possibleCharacters.length();

    for (int i = 0; i < length; ++i) {
        int index = rand() % possibleCharactersLength;
        QChar nextChar = possibleCharacters.at(index);
        token.append(nextChar);
    }

    return token;
}

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
        response = reg(data_client.at(0), data_client.at(1));
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
        response = "Ошибка, функция не определена";
    }
    return QByteArray((response + "\r\n").toUtf8());
}

QByteArray auth(const QString& login, const QString& input_password) {
    // Хэшируем входной пароль с помощью SHA-512
    QString hashedInputPassword = QString(QCryptographicHash::hash(input_password.toUtf8(), QCryptographicHash::Sha512).toHex());

    QVariantMap response;
    QString query = QString("SELECT * FROM Users WHERE login = \"%1\"").arg(login);
    QSqlQuery queryResult = Database::getInstance()->doSQLQuery(query);
    QSqlRecord record = queryResult.record();
    queryResult.first();
    QVariant dbPassword = queryResult.value(record.indexOf("password"));
    if (dbPassword == QVariant::Invalid) {
        response["status"] = "fail 1";
    }
    else if (dbPassword.toString() == hashedInputPassword) { // Сравниваем с хэшированным паролем
        QString token = queryResult.value(record.indexOf("token")).toString();
        response["status"] = "success";
        response["token"] = token;
    }
    else {
        response["status"] = "fail 2";
    }

    qDebug() << QString("Авторизация с логином %1 и паролем %2").arg(login, input_password);

    // Сборка ответа в виде строки
    QString responseString;
    for (auto it = response.constBegin(); it!= response.constEnd(); ++it) {
        responseString += it.key() + ": " + it.value().toString() + "\n";
    }
    return responseString.toUtf8();
}


QByteArray reg(const QString& login, const QString& password) {

    QString token = generateToken();

    // Хэш пароля с помощью SHA-512
    QString hashedPassword = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha512).toHex());

    // Создаем подключение к базе данных
    Database* db = Database::getInstance();

    // Создаем необходимые таблицы, если они не существуют
    db->doSQLQuery("CREATE TABLE IF NOT EXISTS Users (login VARCHAR (255) PRIMARY KEY UNIQUE NOT NULL, password VARCHAR (255) NOT NULL, token VARCHAR (255) UNIQUE NOT NULL)");

    // Выполняем запрос на добавление нового пользователя
    QString query = QString("INSERT INTO Users(login, password, token) "
                            "VALUES (\"%1\", \"%2\", \"%3\")")
                        .arg(login, hashedPassword, token);
    db->doSQLQuery(query);

    return "Registration successful.";
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
