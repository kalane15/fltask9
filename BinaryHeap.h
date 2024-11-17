#pragma once
#include "OpsAndDeps.h"
#define TYPE Request*
#include "stdbool.h"

// Структура для элемента бинарной кучи
typedef struct {
    int key;    // Приоритет
    TYPE data;  // Данные, теперь типа TYPE
} BinaryHeapNode;

// Структура для приоритетной очереди (бинарной кучи)
typedef struct {
    BinaryHeapNode* heap;  // Массив элементов кучи
    bool (*cmp)(int, int);
    int size;                 // Текущий размер кучи
    int capacity;             // Вместимость кучи    
} BinaryHeapPriorityQueue;

void BinaryHeapSwap(BinaryHeapNode* a, BinaryHeapNode* b);

void BinaryHeapHeapify(BinaryHeapPriorityQueue* pq, int index);

kErrors BinaryHeapCreatePriorityQueue(int capacity, BinaryHeapPriorityQueue* pq, bool (*)(int, int));

kErrors BinaryHeapResize(BinaryHeapPriorityQueue* pq);

kErrors BinaryHeapInsert(BinaryHeapPriorityQueue* pq, int key, TYPE data);

kErrors BinaryHeapDeleteMax(BinaryHeapPriorityQueue* pq, Request** max);

kErrors BinaryHeapGetMax(BinaryHeapPriorityQueue* pq, BinaryHeapNode** max);

kErrors BinaryHeapMergeWithoutDestruction(BinaryHeapPriorityQueue* pq1, BinaryHeapPriorityQueue* pq2, BinaryHeapPriorityQueue** merged);

kErrors BinaryHeapMergeWithDestruction(BinaryHeapPriorityQueue* pq1, BinaryHeapPriorityQueue* pq2);

void BinaryHeapFreePriorityQueue(BinaryHeapPriorityQueue* pq);

kErrors BinaryHeapMeld(BinaryHeapPriorityQueue* p_in, BinaryHeapPriorityQueue* p_out);
