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
} LeftistHeapPQ;

LeftistHeapNode* LeftistHeapCreateNode(int key, TYPE data);

LeftistHeapNode* LeftistHeapMerge(LeftistHeapNode* h1, LeftistHeapNode* h2,
                                  bool (*cmp)(int, int));

kErrors LeftistHeapCreatePriorityQueue(LeftistHeapPQ* pq,
                                       bool (*cmp)(int, int));

kErrors LeftistHeapInsert(LeftistHeapPQ* pq, int key, TYPE data);

kErrors LeftistHeapDeleteMax(LeftistHeapPQ* pq,
                             Request** max);

kErrors LeftistHeapGetMax(LeftistHeapPQ* pq, LeftistHeapNode** max);

kErrors LeftistHeapMergeWithDestruction(LeftistHeapPQ* pq1,
                                        LeftistHeapPQ* pq2);

void LeftistHeapFreePriorityQueue(LeftistHeapPQ* pq);

void LeftistHeapPrint(LeftistHeapPQ* p);
kErrors LeftistHeapMeld(LeftistHeapPQ* p_in, LeftistHeapPQ* p_out);
void LeftistHeapPrintNodes(LeftistHeapNode* root, int tab_count);
bool CmpMax(int a, int b);
void LeftistHeapRecursiveFree(LeftistHeapNode* node);
kErrors LeftistHeapCopyPqFromRoot(LeftistHeapPQ* dest, LeftistHeapNode* source);