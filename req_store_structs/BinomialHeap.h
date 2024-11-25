#pragma once
#include "OpsAndDeps.h"

typedef struct BinomialHeapNode {
	int key;
	Request* data;
	struct BinomialHeapNode** children;
	int children_size;
	int children_capacity;
} BinomialHeapNode;


typedef struct BinomialTree {
	int degree;
	BinomialHeapNode* head;
	struct BinomialTree* sibling;
} BinomialTree;


typedef struct BinomialHeapPriorityQueue {
	BinomialTree* root;
	bool (*cmp)(int, int);
	int size;
} BinomialHeapPQ;

BinomialHeapNode* BinomialHeapCreateNode(int key, Request* data);

kErrors BinomialHeapCreateTree(BinomialTree** out, BinomialHeapNode* node);

kErrors BinomialHeapMergeByRootsWithCopy(BinomialTree* h1, BinomialTree* h2, BinomialTree** out, bool(*cmp)(int, int));

kErrors BinomialHeapCreatePriorityQueue(BinomialHeapPQ* out, bool(*inp_cmp)(int, int));

kErrors BinomialHeapInsert(BinomialHeapPQ* heap, int key, Request* data);

kErrors BinomialHeapDeleteMax(BinomialHeapPQ* heap, Request** data);

Request* BinomialHeapGetMax(BinomialHeapPQ* heap);

kErrors BinomialHeapMergeWithDestruction(BinomialHeapPQ* heap1, BinomialHeapPQ* heap2, BinomialHeapPQ* merged);

kErrors BinomialHeapMergeWithoutDestruction(BinomialHeapPQ* pq1, BinomialHeapPQ* pq2, BinomialHeapPQ* merged);

void BinomialHeapFreeRecursively(BinomialHeapNode* node);

void BinomialHeapFreeTree(BinomialTree* tree);

void BinomialHeapFree(BinomialHeapPQ* heap);

void BinomialHeapFreeFromTree(BinomialTree* tree);

kErrors BinomialHeapCopyOneTree(BinomialTree** dest, BinomialTree* source);

kErrors BinomialHeapCopyFromTree(BinomialTree** dest, BinomialTree* source);

kErrors BinomialHeapCopy(BinomialHeapNode** dest, BinomialHeapNode* source);

void PrintChildren(BinomialHeapNode* node);

void BinomialHeapPrint(BinomialHeapPQ* pq);

kErrors BinomialHeapMeld(BinomialHeapPQ* p_in, BinomialHeapPQ* p_out);
