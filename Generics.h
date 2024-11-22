#pragma once
typedef struct Department Department;
typedef struct Request Request;
typedef struct MainModel MainModel;
#include <ctype.h>
#include <float.h>
#include <stdbool.h>
#include "BSTExample.h"
#include "BinomialHeap.h"
#include "Comparators.h"
#include "DynamicArray.h"
#include "time.h"
#include "HashTable.h"
#include "PrefixTree.h"
#include "Treap.h"

kErrors GenericInsertRequest(MainModel* model, Request* req, Department** dep);
void* GenericMallocReqQueue(MainModel* model);

void* GenericMallocDepStruct(MainModel* model);

kErrors GenericCreatePQ(void* queue, bool cmpMax(int a, int b), MainModel* model);

kErrors GenericDeleteMax(void* req_queue, Request** out, MainModel* model);

kErrors GenericCreateDepStruct(void* structure, MainModel* model);

kErrors GenericInsert(void* structure, Department* dep, char* id, MainModel* model);

kErrors GenericMeldReqPq(void* q_from, void* q_to, MainModel* model);

void GenericFreeReqStruct(void* st, ReqStoreType model);

void GenericFreeDepStruct(void* st, MainModel* model);

void GenericFreeAll(MainModel* model);
