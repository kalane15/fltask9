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
} SkewHeapPQ;



SkewHeapNode* SkewHeapCreateNode(int key, TYPESH* data);

kErrors SkewHeapMergeWithoutDestruction(SkewHeapPQ* pq1, SkewHeapPQ* pq2, SkewHeapPQ* merged);

kErrors SkewHeapMergeByRootsWithCopy(SkewHeapNode* h1, SkewHeapNode* h2, SkewHeapNode** out, bool(*cmp)(SkewHeapNode*, SkewHeapNode*));

kErrors SkewHeapCreatePriorityQueue(SkewHeapPQ* out, bool(*inp_cmp)(SkewHeapNode*, SkewHeapNode*));

kErrors SkewHeapInsert(SkewHeapPQ* heap, int key, TYPESH* data);

kErrors SkewHeapDeleteMax(SkewHeapPQ* heap, Request** data);

kErrors SkewHeapDeleteMaxOutNode(SkewHeapPQ* heap, SkewHeapNode** data);

kErrors SkewHeapGetMax(SkewHeapPQ* heap, SkewHeapNode* data);

kErrors SkewHeapMergeWithDestruction(SkewHeapPQ* heap1, SkewHeapPQ* heap2, SkewHeapPQ* merged);

void SkewHeapFreeRecursively(SkewHeapNode* node);

void SkewHeapFree(SkewHeapPQ* heap);

kErrors SkewHeapCopy(SkewHeapNode** dest, SkewHeapNode* source);

kErrors SkewHeapCopyPqFromRoot(SkewHeapPQ* dest, SkewHeapNode* source);

void SkewHeapPrintNode(SkewHeapNode* node);

void SkewHeapPrintNodes(SkewHeapNode* root, int tab_count);

void SkewHeapPrint(SkewHeapPQ* p);

kErrors SkewHeapMeld(SkewHeapPQ* p_in, SkewHeapPQ* p_out);
