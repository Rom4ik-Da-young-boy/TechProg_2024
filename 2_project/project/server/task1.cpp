#include "task1.h"
#include "iostream"

QList<int> divideListInHalf(const QList<int>& list) {
    int size = list.size();
    QList<int> result;
    for (int i = 0; i < size / 2; ++i) {
        result.append(list[i]);
    }
    return result;
}

QByteArray executeTask1() {
    QByteArray response = "Введите размер списка: ";
    std::cout << response.constData();
    std::cout.flush();

    int listSize;
    std::cin >> listSize;

    QList<int> list;
    response = "Введите " + QByteArray::number(listSize) + " чисел для списка: ";
    std::cout << response.constData();
    std::cout.flush();

    for (int i = 0; i < listSize; ++i) {
        int num;
        std::cin >> num;
        list.append(num);
    }

    // Выполните задачу и верните результат
    QList<int> result = divideListInHalf(list);
    response = "Задание завершилось. Результат: ";
    for (int i : result) {
        response += QByteArray::number(i) + " ";
    }
    return response;
}
