#pragma once
#include "OpsAndDeps.h"



typedef struct DANode {
	char* id;
	Department* data;
} DANode;

typedef struct DArray {
	int capacity;
	int size;
	DANode** buffer;
	int (*cmp)(void*, void*);
	int (*cmpById)(char*, char*);
	MainModel* model;
} DArray;



kErrors DArrayCreate(DArray* arr, int (*_cmp)(void*, void*), int (*_cmpById)(char*, char*), MainModel* model);

DANode* DArrayCreateNode(Department* data, const char* id);

kErrors DArrayInsert(DArray* arr, Department* data, const char* id);

void DAFreeNode(DANode* node, MainModel* m);

kErrors DArrayDelete(DArray* arr, char* id);

void DAPrintNode(DANode* node);

void DArrayPrint(DArray* p);

kErrors DAFindIndex(DArray* arr, char* id, int* out);

Department* DArraySearch(DArray* arr, char* id);

void DArrayFree(DArray* arr, MainModel* m);
