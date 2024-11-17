#pragma once
#include "OpsAndDeps.h"
#define TYPE Request*

// Структура для элемента левосторонней кучи
typedef struct LeftistHeapNode {
  int key;                        // Приоритет
  TYPE data;                      // Данные
  struct LeftistHeapNode* left;   // Левый потомок
  struct LeftistHeapNode* right;  // Правый потомок
  int npl;  // Наращиваемая длина пути (null path length)
} LeftistHeapNode;

// Структура для приоритетной очереди (левосторонней кучи)
typedef struct {
  LeftistHeapNode* root;  // Корень кучи
  bool (*cmp)(int, int);
  int size;
} LeftistHeapPriorityQueue;

LeftistHeapNode* LeftistHeapCreateNode(int key, TYPE data);

LeftistHeapNode* LeftistHeapMerge(LeftistHeapNode* h1, LeftistHeapNode* h2,
                                  bool (*cmp)(int, int));

kErrors LeftistHeapCreatePriorityQueue(LeftistHeapPriorityQueue* pq,
                                       bool (*cmp)(int, int));

kErrors LeftistHeapInsert(LeftistHeapPriorityQueue* pq, int key, TYPE data);

kErrors LeftistHeapDeleteMax(LeftistHeapPriorityQueue* pq,
                             Request** max);

kErrors LeftistHeapGetMax(LeftistHeapPriorityQueue* pq, LeftistHeapNode** max);

kErrors LeftistHeapMergeWithDestruction(LeftistHeapPriorityQueue* pq1,
                                        LeftistHeapPriorityQueue* pq2);

void LeftistHeapFreePriorityQueue(LeftistHeapPriorityQueue* pq);

void LeftistHeapPrint(LeftistHeapPriorityQueue* p);
kErrors LeftistHeapMeld(LeftistHeapPriorityQueue* p_in, LeftistHeapPriorityQueue* p_out);
void LeftistHeapPrintNodes(LeftistHeapNode* root, int tab_count);
bool CmpMax(int a, int b);
void LeftistHeapRecursiveFree(LeftistHeapNode* node);
kErrors LeftistHeapCopyPqFromRoot(LeftistHeapPriorityQueue* dest, LeftistHeapNode* source);