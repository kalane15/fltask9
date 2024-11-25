#pragma once
#include "myerrors.h"
#include "stdlib.h"
#include "OpsAndDeps.h"
typedef struct node {
	struct node* left;
	struct node* right;
	int x, y;
	Request* data;
} TreapNode;

typedef struct Treap {
	int size;
	TreapNode* root;
} Treap;

kErrors TreapCreate(Treap* t);

kErrors TreapSplit(Treap* to_split, int key, Treap** l, Treap** r);

kErrors TreapCreateNode(TreapNode** out, int y, Request* data);

int GetX(TreapNode* t);

void TreapSplitRecursive(TreapNode* t, int x0, TreapNode** l, TreapNode** r);

void TreapNodeUpdate(TreapNode* t);

TreapNode* TreapMergeRecursive(TreapNode* l, TreapNode* r);

kErrors TreapCopyNodes(TreapNode** dest, TreapNode* source);

kErrors TreapCopy(Treap* dest, Treap* source);

kErrors TreapMergeWithoutDestruction(Treap* t1, Treap* t2, Treap** merged);

kErrors TreapMergeWithDestruction(Treap* t1, Treap* t2, Treap** merged);

kErrors TreapInsertToRoot(TreapNode** root, int p, Request* data);

kErrors TreapInsert(Treap* treap, int p, Request* data);

void TreapFree(Treap* treap);

void TreapFreeRecursive(TreapNode* node);

kErrors TreapDeleteMax(Treap* treap, Request** out);

kErrors TreapMeld(Treap* p_in, Treap* p_out);

void TreapPrintNode(TreapNode* root);

void TreapPrintNodes(TreapNode* root, int tab_count);

void TreapPrint(Treap* p);

Request* TreapGetMax(Treap* heap);
