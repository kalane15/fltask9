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
#include <stdio.h>
#include <stdlib.h>
#include "LeftistHeap.h"


bool CmpMax(int a, int b) {
    return a < b;
}

LeftistHeapNode* LeftistHeapCreateNode(int key, TYPE data) {
    LeftistHeapNode* node = (LeftistHeapNode*)malloc(sizeof(LeftistHeapNode));
    if (!node) return NULL;

    node->key = key;
    RequestCopy(&node->data, data);
    node->left = NULL;
    node->right = NULL;
    node->npl = 0;  // Начальная нпл (null path length)
    return node;
}

kErrors LeftistHeapCopyPqFromRoot(LeftistHeapPQ* dest, LeftistHeapNode* source) {
    if (source == NULL) {
        return SUCCESS;
    }
    kErrors status = SUCCESS;
    status = LeftistHeapCopyPqFromRoot(dest, source->left);    
    if (status != SUCCESS) {
        return status;
    }
    status = LeftistHeapInsert(dest, source->key, source->data);
    if (status != SUCCESS) {
        return status;
    }
    status = LeftistHeapCopyPqFromRoot(dest, source->right);
    if (status != SUCCESS) {
        return status;
    }
}
// Функция для слияния двух левосторонних куч
LeftistHeapNode* LeftistHeapMerge(LeftistHeapNode* h1, LeftistHeapNode* h2, bool (*cmp)(int, int)) {
    if (!h1) return h2;
    if (!h2) return h1;

    if (cmp(h1->key, h2->key)) {
        // Обмен значениями h1 и h2
        LeftistHeapNode* temp = h1;
        h1 = h2;
        h2 = temp;
    }

    // Слияние правого поддерева
    h1->right = LeftistHeapMerge(h1->right, h2, cmp);

    // Проверка условия левосторонности
    if (!h1->left || (h1->left->npl < h1->right->npl)) {
        // Меняем местами левое и правое поддерево
        LeftistHeapNode* temp = h1->left;
        h1->left = h1->right;
        h1->right = temp;
    }

    // Обновляем npl
    if (!h1->right) {
        h1->npl = 0;
    }
    else {
        h1->npl = h1->right->npl + 1;
    }

    return h1;
}

// Функция для создания пустой левосторонней кучи
kErrors LeftistHeapCreatePriorityQueue(LeftistHeapPQ* pq, bool (*inp_cmp)(int, int)) {
    pq->root = NULL;
    pq->cmp = inp_cmp;
    pq->size = 0;
    return SUCCESS;
}

// Вставка элемента в левостороннюю кучу
kErrors LeftistHeapInsert(LeftistHeapPQ* pq, int key, TYPE data) {
    LeftistHeapNode* newNode = LeftistHeapCreateNode(key, data);
    if (!newNode) return MEM_ALLOC_ERR;

    newNode->left = NULL;
    newNode->right = NULL;
    pq->root = LeftistHeapMerge(pq->root, newNode, pq->cmp);
    pq->size++;
    return SUCCESS;
}

// Удаление максимального элемента из кучи
kErrors LeftistHeapDeleteMax(LeftistHeapPQ* pq, Request** max) {
    if (!pq->root) return INC_INP_DATA;  // Куча пуста

    LeftistHeapNode* root = pq->root;
    RequestCopy(max, pq->root->data);
    
    pq->root = LeftistHeapMerge(pq->root->left, pq->root->right, pq->cmp);
    pq->size--;
    free(root);
    // TO DO когда передают текущую заявку в основном цикле, ее фришат и она дальше работает неккоректно крч нормально освобождать память
    return SUCCESS;
}

// Поиск максимального элемента в куче
kErrors LeftistHeapGetMax(LeftistHeapPQ* pq, LeftistHeapNode** max) {
    if (!pq->root) return INC_INP_DATA;  // Куча пуста

    *max = pq->root;
    return SUCCESS;
}

// Слияние двух очередей без разрушения исходных
kErrors LeftistHeapMergeWithoutDestruction(LeftistHeapPQ* pq1, LeftistHeapPQ* pq2, LeftistHeapPQ* merged) {
    kErrors error = LeftistHeapCreatePriorityQueue(merged, pq1->cmp);
    if (error != SUCCESS) return error;

    LeftistHeapCopyPqFromRoot(merged, pq1->root);
    merged->root = LeftistHeapMerge(merged->root, pq2->root, pq1->cmp);
    merged->size = pq1->size + pq2->size;
    return SUCCESS;
}

// Слияние двух очередей с разрушением исходных
kErrors LeftistHeapMergeWithDestruction(LeftistHeapPQ* pq1, LeftistHeapPQ* pq2) {
    kErrors status = SUCCESS; //TO DO: add copy res to merged
    LeftistHeapPQ merged;
    status = LeftistHeapCreatePriorityQueue(&merged, pq1->cmp);
    if (status != SUCCESS) {
        return status;
    };

    status = LeftistHeapMergeWithoutDestruction(pq1, pq2, &merged);
    if (status != SUCCESS) {
        LeftistHeapFreePriorityQueue(&merged);
        return status;
    }  

    status = LeftistHeapCopyPqFromRoot(pq1, merged.root);
    if (status != SUCCESS) {
        LeftistHeapFreePriorityQueue(&merged);
        return status;
    }
    LeftistHeapFreePriorityQueue(&merged);
    return SUCCESS;
}

void LeftistHeapRecursiveFree(LeftistHeapNode* node) {
    if (node == NULL) {
        return;
    }
    if (node->left != NULL) {
        LeftistHeapRecursiveFree(node->left);
    }
    if (node->right != NULL) {
        LeftistHeapRecursiveFree(node->right);
    }
    free(node);
}

// Освобождение памяти
void LeftistHeapFreePriorityQueue(LeftistHeapPQ* pq) {
    LeftistHeapRecursiveFree(pq->root);
}

void LeftistHeapPrintNode(LeftistHeapNode* node) {
    printf("%d npl:%d\n", node->key, node->npl);
}

void LeftistHeapPrintNodes(LeftistHeapNode* root, int tab_count) {
    if (root != NULL)
    {
        LeftistHeapPrintNodes(root->right, tab_count + 1);
        for (int i = 0; i < tab_count; ++i) { printf("\t"); }
        LeftistHeapPrintNode(root);
        LeftistHeapPrintNodes(root->left, tab_count + 1);
    }
}

void LeftistHeapPrint(LeftistHeapPQ* p)
{
    LeftistHeapPrintNodes(p->root, 0);
}

kErrors LeftistHeapMeld(LeftistHeapPQ* p_in, LeftistHeapPQ* p_out) {
    kErrors status = SUCCESS;
    LeftistHeapNode* cur = NULL;
    Request* cur_req = NULL;
    while (p_in->size > 0) {
        status = LeftistHeapDeleteMax(p_in, &cur_req);
        if (status != SUCCESS) {
            return status;
        }
        status = LeftistHeapInsert(p_out, cur_req->priority, cur_req);
        if (status != SUCCESS) {
            return status;
        }
    }
    return SUCCESS;
}