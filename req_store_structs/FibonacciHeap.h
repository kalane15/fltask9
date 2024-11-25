#pragma once
#include "stdlib.h"
#include "stdbool.h"
#include "myerrors.h"
#include "OpsAndDeps.h"

typedef struct FibHeapNode {
	int key;
	struct FibHeapNode* parent;
	struct FibHeapNode* child;
	struct FibHeapNode* left;
	struct FibHeapNode* right;
	int degree;
	Request* data;
	bool mark;
} FibHeapNode;

typedef struct FibHeapPQ {
	int size;
	FibHeapNode* min;
} FibHeapPQ;

kErrors FibHeapCreate(FibHeapPQ* pq);

kErrors FibHeapCreateNode(FibHeapNode** out, int key, Request* data);

kErrors FibHeapInsert(FibHeapPQ* heap, int key, Request* data);

Request* GetMax(FibHeapPQ* heap);

kErrors FibHeapUnionLists(FibHeapNode* first, FibHeapNode* second);

void FibHeapPrintTree(FibHeapNode* root);

void FibHeapPrint(FibHeapPQ* heap);

kErrors FibHeapCopyFromRoot(FibHeapNode** dest, FibHeapNode* source, FibHeapNode* dparent);

kErrors FibHeapCopy(FibHeapPQ* dest, FibHeapPQ* source);

kErrors FibHeapDeleteMax(FibHeapPQ* heap, Request** out);

kErrors FibHeapConsolidate(FibHeapPQ* heap);

kErrors FibHeapMerge(FibHeapPQ* h1, FibHeapPQ* h2, FibHeapPQ** merged);

kErrors FibHeapMergeWithoutDestruction(FibHeapPQ* h1, FibHeapPQ* h2, FibHeapPQ** merged);

kErrors FibHeapMergeWithDestruction(FibHeapPQ* h1, FibHeapPQ* h2, FibHeapPQ** merged);

void FibHeapFreeRecursive(FibHeapNode* node);

void FibHeapFree(FibHeapPQ* heap);

kErrors FibHeapMeld(FibHeapPQ* p_in, FibHeapPQ* p_out);
