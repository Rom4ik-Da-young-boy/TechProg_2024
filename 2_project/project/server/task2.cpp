#include "task2.h"
#include <QtCore>
#include <QMap>
#include <QSet>
#include <iostream>
#include <limits.h> // INT_MAX constant
#include <cmath>

#include "task2.h"

QByteArray executeTask2(int numVertices, const QMap<int, std::pair<int, int>>& vertexCoordinates) {
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

    // Получаем кратчайшие расстояния
    QMap<int, int> distances = graph.shortestPath(1);

    // Формируем ответ
    QByteArray response = "Задание завершилось. Кратчайшее расстояние:";
    bool firstVertex = true;
    for (auto it = distances.begin(); it != distances.end(); ++it) {
        if (!firstVertex) {
            response += "\n"; // Добавляем символ новой строки перед каждой вершиной, кроме первой
        } else {
            firstVertex = false;
        }
        response += "Вершина " + QByteArray::number(it.key()) + ": " + QByteArray::number(it.value()) + " ";
    }
    return response;
}

QMap<int, int> Graph::shortestPath(int startVertex) {
    // Инициализация карты расстояний, где ключ - вершина, а значение - расстояние до нее
    QMap<int, int> distances;
    for (int i = 1; i <= numVertices; ++i) {
        distances[i] = INT_MAX; // Устанавливаем расстояние до всех вершин как бесконечность
    }
    distances[startVertex] = 0; // Расстояние до начальной вершины равно 0

    // Множество, в котором будут храниться вершины, расстояния до которых уже известны
    QSet<int> visitedVertices;

    // Основной цикл алгоритма Дейкстры
    while (visitedVertices.size() < numVertices) {
        int minDistance = INT_MAX;
        int minVertex;

        // Находим вершину с наименьшим расстоянием из тех, которые еще не были посещены
        for (int v = 1; v <= numVertices; ++v) {
            if (!visitedVertices.contains(v) && distances[v] < minDistance) {
                minDistance = distances[v];
                minVertex = v;
            }
        }

        // Добавляем найденную вершину в посещенные
        visitedVertices.insert(minVertex);

        // Обновляем расстояния до смежных вершин
        for (auto it = adjacencyList[minVertex].begin(); it != adjacencyList[minVertex].end(); ++it) {
            int to = it.key();
            int weight = it.value();
            if (distances[minVertex] != INT_MAX && distances[minVertex] + weight < distances[to]) {
                distances[to] = distances[minVertex] + weight;
            }
        }
    }

    return distances;
}
