#pragma once
#include "BinaryHeap.h"
#include "LeftistHeap.h"
#include "SkewHeap.h"

typedef struct BSTNode {
	Department* data;
	struct BSTNode* left;
	struct BSTNode* right;
} BSTNode;

typedef struct BST {
	BSTNode* root;
	int (*cmpLess)(char*, char*);
	MainModel* model;
} BST;

kErrors BSTCreate(BST* tree, int (*_cmp)(char*, char*), MainModel* model);

BSTNode* BSTCreateNode(Department* data);

kErrors BSTInsert(BST* tree, Department* data);

void BSTFreeNode(BSTNode* node, MainModel* model);

kErrors BSTDelete(BST* tree, char* id);

void BSTPrintNode(BSTNode* root);

void BSTPrintNodes(BSTNode* root, int tab_count);

void BSTPrint(BST* p);

Department* BSTSearch(BST* tree, char* id);

void BSTFreeTreeRecursive(BSTNode* node, MainModel* model);

void BSTFree(BST* tree);
