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
} BinomialHeapPriorityQueue;

BinomialHeapNode* BinomialHeapCreateNode(int key, Request* data);

kErrors BinomialHeapCreateTree(BinomialTree** out, BinomialHeapNode* node);

kErrors BinomialHeapMergeByRootsWithCopy(BinomialTree* h1, BinomialTree* h2, BinomialTree** out, bool(*cmp)(int, int));

kErrors BinomialHeapMergeByRootsWithCopy(BinomialHeapPriorityQueue* h1, BinomialHeapPriorityQueue* h2, BinomialHeapPriorityQueue** out, bool(*cmp)(int, int));

kErrors BinomialHeapCreatePriorityQueue(BinomialHeapPriorityQueue* out, bool(*inp_cmp)(int*, int*));

kErrors BinomialHeapInsert(BinomialHeapPriorityQueue* heap, int key, Request* data);

kErrors BinomialHeapDeleteMax(BinomialHeapPriorityQueue* heap, Request** data);

void BinomialHeapFreeRecursively(BinomialHeapNode* node);

void BinomialHeapFreeTree(BinomialTree* tree);

void BinomialHeapFree(BinomialHeapPriorityQueue* heap);

void BinomialHeapFreeFromTree(BinomialTree* tree);

kErrors BinomialHeapCopyOneTree(BinomialTree** dest, BinomialTree* source);

kErrors BinomialHeapCopyFromTree(BinomialTree** dest, BinomialTree* source);

kErrors BinomialHeapCopy(BinomialHeapNode** dest, BinomialHeapNode* source);

void PrintChildren(BinomialHeapNode* node);

void BinomialHeapPrint(BinomialHeapPriorityQueue* pq);

kErrors BinomialHeapMeld(BinomialHeapPriorityQueue* p_in, BinomialHeapPriorityQueue* p_out);
