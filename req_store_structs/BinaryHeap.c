#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include "locale.h"
#include "limits.h"
#include "stdarg.h"
#include "stdint.h"
#include "stdio.h"
#include "myerrors.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "BinaryHeap.h"


// В данном случае, тип данных это int, но можно заменить на любой тип, например, float, char и т.д.



// Функции для работы с кучей
void BinaryHeapSwap(BinaryHeapNode* a, BinaryHeapNode* b) {
    BinaryHeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void BinaryHeapHeapify(BinaryHeapPriorityQueue* pq, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    // Если левый потомок больше текущего узла
    if (left < pq->size && pq->cmp(pq->heap[left].key, pq->heap[largest].key)) {
        largest = left;
    }
    // Если правый потомок больше текущего или левого узла
    if (right < pq->size && pq->cmp(pq->heap[right].key, pq->heap[largest].key)) {
        largest = right;
    }

    // Если наибольший элемент не текущий узел
    if (largest != index) {
        // Меняем местами текущий узел и наибольший потомок
        BinaryHeapSwap(&pq->heap[index], &pq->heap[largest]);
        // Рекурсивно восстанавливаем кучку для поддерева
        BinaryHeapHeapify(pq, largest);
    }
}

kErrors BinaryHeapCreatePriorityQueue(int capacity, BinaryHeapPriorityQueue* pq, bool (*inp_cmp)(int, int)) {
    if (pq == NULL) {
        return MEM_ALLOC_ERR;
    }

    pq->heap = (BinaryHeapNode*)malloc(sizeof(BinaryHeapNode) * capacity);
    if (pq->heap == NULL) {
        free(pq);
        return MEM_ALLOC_ERR;
    }

    pq->size = 0;
    pq->capacity = capacity;
    pq->cmp = inp_cmp;
    return SUCCESS;
}

kErrors BinaryHeapResize(BinaryHeapPriorityQueue* pq) {
    pq->capacity *= 2;
    pq->heap = (BinaryHeapNode*)realloc(pq->heap, sizeof(BinaryHeapNode) * pq->capacity);
    if (pq->heap == NULL) {
        return MEM_ALLOC_ERR;
    }
    return SUCCESS;
}

kErrors BinaryHeapInsert(BinaryHeapPriorityQueue* pq, int key, TYPE data) {
    if (pq->size == pq->capacity) {
        kErrors error = BinaryHeapResize(pq);
        if (error != SUCCESS) {
            return error;
        }
    }

    int index = pq->size++;
    pq->heap[index].key = key;
    pq->heap[index].data = data;

    // Поднимаем элемент вверх, пока не выполнится условие кучи
    while (index > 0 && pq->heap[(index - 1) / 2].key < pq->heap[index].key) {
        BinaryHeapSwap(&pq->heap[index], &pq->heap[(index - 1) / 2]);
        index = (index - 1) / 2;
    }

    return SUCCESS;
}

kErrors BinaryHeapDeleteMax(BinaryHeapPriorityQueue* pq, Request** max) {
    if (pq->size == 0) {
        return INC_INP_DATA;  // Нет элементов для удаления
    }

    *max = pq->heap[0].data;
    pq->heap[0] = pq->heap[--pq->size];
    BinaryHeapHeapify(pq, 0);

    return SUCCESS;
}

kErrors BinaryHeapGetMax(BinaryHeapPriorityQueue* pq, BinaryHeapNode** max) {
    if (pq->size == 0) {
        return INC_INP_DATA;  // Очередь пуста
    }

    *max = &pq->heap[0];
    return SUCCESS;
}

// Слияние двух очередей без разрушения исходных
kErrors BinaryHeapMergeWithoutDestruction(BinaryHeapPriorityQueue* pq1, BinaryHeapPriorityQueue* pq2, BinaryHeapPriorityQueue** merged) {
    *merged = NULL;
    kErrors error = BinaryHeapCreatePriorityQueue(pq1->size + pq2->size, merged, pq1->cmp);
    if (error != SUCCESS) {
        return error;
    }

    // Копируем элементы из первой очереди
    for (int i = 0; i < pq1->size; i++) {
        error = BinaryHeapInsert(*merged, pq1->heap[i].key, pq1->heap[i].data);
        if (error != SUCCESS) {
            BinaryHeapFreePriorityQueue(*merged);
            return error;
        }
    }

    // Копируем элементы из второй очереди
    for (int i = 0; i < pq2->size; i++) {
        error = BinaryHeapInsert(*merged, pq2->heap[i].key, pq2->heap[i].data);
        if (error != SUCCESS) {
            BinaryHeapFreePriorityQueue(*merged);
            return error;
        }
    }

    return SUCCESS;
}

// Слияние двух очередей с разрушением исходных
kErrors BinaryHeapMergeWithDestruction(BinaryHeapPriorityQueue* pq1, BinaryHeapPriorityQueue* pq2) {
    kErrors error;
    for (int i = 0; i < pq2->size; i++) {
        error = BinaryHeapInsert(pq1, pq2->heap[i].key, pq2->heap[i].data);
        if (error != SUCCESS) {
            return error;
        }
    }

    free(pq2->heap);
    free(pq2);
    pq2 = NULL;

    return SUCCESS;
}

// Функция для освобождения памяти
void BinaryHeapFreePriorityQueue(BinaryHeapPriorityQueue* pq) {
    free(pq->heap);
    free(pq);
}

kErrors BinaryHeapMeld(BinaryHeapPriorityQueue* p_in, BinaryHeapPriorityQueue* p_out) {
    kErrors status = SUCCESS;
    BinaryHeapNode* cur = NULL;
    Request* cur_req = NULL;
    while (p_in->size > 0) {
        status = BinaryHeapDeleteMax(p_in, &cur_req);
        if (status != SUCCESS) {
            return status;
        }
        status = BinaryHeapInsert(p_out, cur_req->priority, cur_req);
        if (status != SUCCESS) {
            return status;
        }
    }
    return SUCCESS;
}