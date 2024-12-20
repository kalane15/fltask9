﻿#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include "locale.h"
#include "limits.h"
#include "stdarg.h"
#include "stdint.h"
#include "stdio.h"
#include "myerrors.h"
#include "string.h"
#include "BinaryHeap.h"
#include "LeftistHeap.h"
#include "SkewHeap.h"
#include "DynamicArray.h"
#include "mylib.h"
#include "time.h"
#include "InputParsing.h"
#include "BSTExample.h"
#include "Comparators.h"
#include "OpsAndDeps.h"
#include "OperateRequests.h"
#include "vector.h"
#include "Generics.h"
#include "BinomialHeap.h"
#include "Treap.h"
#include "FibonacciHeap.h"

int main(int argsc, char** args) {
	srand(time(NULL));
	kErrors status = SUCCESS;
	int max_priority;
	ReqStoreType req_store_type;
	DepStoreType dep_store_type;
	time_t modelling_start; 
	time_t modelling_finished;
	int min_op_work;
	int max_op_work; 
	int deps_count;
	MainModel main_model;
	status = ParseInput(argsc, args, &max_priority, &req_store_type, &dep_store_type,
		&modelling_start, &modelling_finished, &min_op_work, &max_op_work, &deps_count, &main_model);
	if (status != SUCCESS) {
		printf("%d\n", status);
		return status;
	}
	status = ParseRequests(argsc, args, &main_model);
	if (status != SUCCESS) {
		printf("%d\n", status);
		return status;
	}
	
	FILE* out = fopen("out.txt", "w");
	status = SimulateModel(&main_model, out);
	if (status != SUCCESS) {
		fclose(out);
		printf("%d\n", status);
		return status;
	}
	GenericFreeAll(&main_model);
	fclose(out);
	return 0;
}