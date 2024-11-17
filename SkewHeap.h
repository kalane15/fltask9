#pragma once
#include "myerrors.h"
#include "stdbool.h"
#include "time.h"
#include "OpsAndDeps.h"

typedef struct SkewHeapNode {
    TYPESH* data; // Данные, связанные с приоритетом
    struct SkewHeapNode* left;
    struct SkewHeapNode* right;
} SkewHeapNode;

typedef struct SkewHeapPriorityQueue {
    SkewHeapNode* root;
    bool (*cmp)(int, int);
    int size;
} SkewHeapPriorityQueue;



SkewHeapNode* SkewHeapCreateNode(int key, TYPESH* data);

kErrors SkewHeapMergeWithoutDestruction(SkewHeapPriorityQueue* pq1, SkewHeapPriorityQueue* pq2, SkewHeapPriorityQueue* merged);

kErrors SkewHeapMergeByRootsWithCopy(SkewHeapNode* h1, SkewHeapNode* h2, SkewHeapNode** out, bool(*cmp)(SkewHeapNode*, SkewHeapNode*));

kErrors SkewHeapCreatePriorityQueue(SkewHeapPriorityQueue* out, bool(*inp_cmp)(SkewHeapNode*, SkewHeapNode*));

kErrors SkewHeapInsert(SkewHeapPriorityQueue* heap, int key, TYPESH* data);

kErrors SkewHeapDeleteMax(SkewHeapPriorityQueue* heap, Request** data);

kErrors SkewHeapDeleteMaxOutNode(SkewHeapPriorityQueue* heap, SkewHeapNode** data);

kErrors SkewHeapGetMax(SkewHeapPriorityQueue* heap, SkewHeapNode* data);

kErrors SkewHeapMergeWithDestruction(SkewHeapPriorityQueue* heap1, SkewHeapPriorityQueue* heap2, SkewHeapPriorityQueue* merged);

void SkewHeapFreeRecursively(SkewHeapNode* node);

void SkewHeapFree(SkewHeapPriorityQueue* heap);

kErrors SkewHeapCopy(SkewHeapNode** dest, SkewHeapNode* source);

kErrors SkewHeapCopyPqFromRoot(SkewHeapPriorityQueue* dest, SkewHeapNode* source);

void SkewHeapPrintNode(SkewHeapNode* node);

void SkewHeapPrintNodes(SkewHeapNode* root, int tab_count);

void SkewHeapPrint(SkewHeapPriorityQueue* p);

kErrors SkewHeapMeld(SkewHeapPriorityQueue* p_in, SkewHeapPriorityQueue* p_out);
