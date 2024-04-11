#ifndef TASK2_H
#define TASK2_H

#include <QtCore>
#include <QMap>
#include <QSet>

QByteArray executeTask2(int numVertices, const QMap<int, std::pair<int, int>>& vertexCoordinates);


class Graph {
public:
    Graph(int vertices) : numVertices(vertices) {}

    // Добавление ребра в граф
    void addEdge(int from, int to, int weight) {
        adjacencyList[from][to] = weight;
        adjacencyList[to][from] = weight; // Граф без направлений
    }

    // Поиск кратчайшего расстояния от заданной вершины до всех остальных
    QMap<int, int> shortestPath(int startVertex);

private:
    int numVertices;
    QMap<int, QMap<int, int>> adjacencyList;
};

#endif // TASK2_H
