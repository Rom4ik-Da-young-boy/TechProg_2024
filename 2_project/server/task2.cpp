#include "task2.h"
#include <QtCore>
#include <QMap>
#include <QSet>
#include <iostream>
#include <limits.h> //  INT_MAX constant


QMap<int, int> Graph::shortestPath(int startVertex) {
    // Initialize the distances map with infinite distances for all vertices except the start vertex
    QMap<int, int> distances;
    for (int i = 1; i <= numVertices; ++i) {
        distances[i] = INT_MAX;
    }
    distances[startVertex] = 0;

    // Create a set to keep track of the vertices that have been visited
    QSet<int> visited;

    // Loop until all vertices have been visited
    while (!visited.contains(numVertices)) {
        // Find the vertex with the smallest distance that has not been visited yet
        int currentVertex = -1;
        int smallestDistance = INT_MAX;
        for (int i = 1; i <= numVertices; ++i) {
            if (!visited.contains(i) && distances[i] < smallestDistance) {
                currentVertex = i;
                smallestDistance = distances[i];
            }
        }

        // If no such vertex is found, break the loop
        if (currentVertex == -1) {
            break;
        }

        // Mark the current vertex as visited
        visited.insert(currentVertex);

        // Update the distances to the neighbors of the current vertex
        for (auto it = adjacencyList[currentVertex].begin(); it!= adjacencyList[currentVertex].end(); ++it) {
            int neighbor = it.key();
            int weight = it.value();

            // If the neighbor has not been visited yet and the distance to the neighbor can be improved
            if (!visited.contains(neighbor) && distances[currentVertex] + weight < distances[neighbor]) {
                distances[neighbor] = distances[currentVertex] + weight;
            }
        }
    }

    return distances;
}

int mainGraph() {
    int numVertices;
    std::cout << "Введите количество вершин: ";
    std::cin >> numVertices;

    Graph graph(numVertices);

    // Ввод координат для каждой вершины
    QMap<int, std::pair<int, int>> vertexCoordinates;
    for (int i = 1; i <= numVertices; ++i) {
        int x, y;
        std::cout << "Введите координаты вершины " << i << "(x y): ";
        std::cin >> x >> y;
        vertexCoordinates[i] = std::make_pair(x, y);
    }

    // Добавление ребер в граф
    for (int i = 1; i <= numVertices; ++i) {
        for (int j = i + 1; j <= numVertices; ++j) {
            int weight = std::sqrt(std::pow(vertexCoordinates[i].first - vertexCoordinates[j].first, 2) +
                                   std::pow(vertexCoordinates[i].second - vertexCoordinates[j].second, 2));
            graph.addEdge(i, j, weight);
        }
    }

    int startVertex;
    std::cout << "Введите вершину с которой хотите начать: ";
    std::cin >> startVertex;

    QMap<int, int> distances = graph.shortestPath(startVertex);

    // Вывод кратчайших расстояний до всех вершин от начальной вершины
    qDebug() << "Кратчайшее расстояние от начальной вершины" << startVertex << ":";

    for (auto it = distances.begin(); it!= distances.end(); ++it) {
        qDebug() << "До вершины" << it.key() << ":" << it.value();
    }

    return 0;
}
