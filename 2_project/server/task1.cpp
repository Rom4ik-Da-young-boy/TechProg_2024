#include "task1.h"

template <typename T>
QList<T> divideListInHalf(const QList<T>& list) {
    int size = list.size();
    int half = size / 2;
    return QList<T>(list.begin(), list.begin() + half);
}
