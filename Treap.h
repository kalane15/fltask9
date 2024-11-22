#pragma once
#include "myerrors.h"
#include "stdlib.h"
#include "OpsAndDeps.h"
typedef struct node {
	struct node* left;
	struct node* right;
	int x, y;
	Request* data;
} node;

typedef struct Treap {
	int size;
	node* root;
} Treap;

kErrors TreapCreate(Treap* t);

kErrors TreapSplit(Treap* to_split, int key, Treap** l, Treap** r);

kErrors TreapCreateNode(node** out, int y, Request* data);

int GetX(node* t);

void TreapSplitRecursive(node* t, int x0, node** l, node** r);

void TreapNodeUpdate(node* t);

node* TreapMergeRecursive(node* l, node* r);

kErrors TreapCopyNodes(node** dest, node* source);

kErrors TreapCopy(Treap* dest, Treap* source);

kErrors TreapMergeWithoutDestruction(Treap* t1, Treap* t2, Treap** merged);

kErrors TreapMergeWithDestruction(Treap* t1, Treap* t2, Treap** merged);

kErrors TreapInsertToRoot(node** root, int p, Request* data);

kErrors TreapInsert(Treap* treap, int p, Request* data);

void TreapFree(Treap* treap);

void TreapFreeRecursive(node* node);

kErrors TreapDeleteMax(Treap* treap, Request** out);

kErrors TreapMeld(Treap* p_in, Treap* p_out);

void TreapPrintNode(node* root);

void TreapPrintNodes(node* root, int tab_count);

void TreapPrint(Treap* p);

Request* TreapGetMax(Treap* heap);
