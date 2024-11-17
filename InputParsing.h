#pragma once
typedef struct Department Department;
typedef struct MainModel MainModel;
#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include "myerrors.h"

typedef enum ReqStoreType {
	BINARY_HEAP,
	BINOMIAL_HEAP,
	FIB_HEAP,
	SKEW_HEAP,
	LEFTIST_HEAP,
	TREAP
} ReqStoreType;

typedef enum DepStoreType {
	HAST_SET,
	DYNAMIC_ARRAY,
	BINARY_SEARCH_TREE,
	TRIE
} DepStoreType;
#include "OpsAndDeps.h"

kErrors SwitchReqStoreType(char* s, ReqStoreType* req_store_type);

kErrors SwitchDepStoreType(char* s, DepStoreType* req_store_type);

kErrors ParseTime(FILE* config, char* temp, time_t* model_start, time_t* model_finished);

kErrors ParseOpsAndDepsCount(FILE* config, char* temp, int* min_op_work, int* max_op_work, int* deps_count);

kErrors ParseInput(int argsc, char** args, int* max_priority, ReqStoreType* req_store_type, 
	DepStoreType* dep_store_type, time_t* modelling_start, time_t* modelling_finished, 
	int* min_op_work, int* max_op_work, int* deps_count, MainModel* m);
