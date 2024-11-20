#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "stdlib.h"
#include "stdbool.h"
#include "myerrors.h"
#include "Comparators.h"
#include "mylib.h"
#include "Generics.h"
typedef struct Request Request;

kErrors DepCreate(int op_count, int overload_coeff, Department** out, char* id, MainModel* model) {
	kErrors status = SUCCESS;
	Department* dep = (Department*)calloc(1, sizeof(Department));
	if (dep == NULL) {
		return MEM_ALLOC_ERR;
	}
	dep->op_count = op_count;
	dep->overload_coeff = overload_coeff;

	dep->dep_id = (char*)malloc(1024);
	if (dep->dep_id == NULL) {
		free(dep);
		return MEM_ALLOC_ERR;
	}
	strcpy(dep->dep_id, id);

	dep->req_queue = GenericMallocReqQueue(model);
	if (dep->req_queue == NULL) {
		free(dep);
		free(dep->dep_id);
		return MEM_ALLOC_ERR;
	}

	status = GenericCreatePQ(dep->req_queue, cmpMax, model);
	if (status != SUCCESS) {
		free(dep->req_queue);
		free(dep);
	}
	dep->operators = (Operator*)calloc(op_count, sizeof(Operator));

	*out = dep;
	return SUCCESS;
}

kErrors RequestCreate(int priority, char* id, char* text, char* time, Request** out, int r_id, MainModel* model) {
	*out = (Request*)malloc(sizeof(Request));
	if (*out == NULL) {
		return MEM_ALLOC_ERR;
	}
	Request* _out = *out;
	_out->dep_id = (char*)malloc(strlen(id) + 1);
	if (_out->dep_id == NULL) {
		return MEM_ALLOC_ERR;
	}
	strcpy(_out->dep_id, id);
	_out->text = (char*)malloc(strlen(text) + 1);
	if (_out->text == NULL) {
		return MEM_ALLOC_ERR;
	}
	strcpy(_out->text, text);

	_out->req_time_get = ConvertToTm(time);
	_out->priority = priority;
	_out->r_id = r_id;
	_out->time_to_operate = RandomNumber(model->min_op_work, model->max_op_work);
	return SUCCESS;
}

kErrors RequestCopy(Request** dest, Request* source) {
	if (source == NULL) {
		*dest = NULL;
		return SUCCESS;
	}
	*dest = (Request*)malloc(sizeof(Request));
	if (*dest == NULL) {
		return MEM_ALLOC_ERR;
	}
	(*dest)->dep_id = (char*)malloc(strlen(source->dep_id) + 1);
	if ((*dest)->dep_id == NULL) {
		free((*dest));
		return MEM_ALLOC_ERR;
	}
	strcpy((*dest)->dep_id, source->dep_id);
	(*dest)->priority = source->priority;
	(*dest)->text = (char*)malloc(strlen(source->text) + 1);
	if ((*dest)->text == NULL) {
		free((*dest)->dep_id);
		free((*dest));
		return MEM_ALLOC_ERR;
	}
	strcpy((*dest)->text, source->text);
	(*dest)->req_time_get = source->req_time_get;
	(*dest)->r_id = source->r_id;
	(*dest)->time_to_operate = source->time_to_operate;
	return SUCCESS;
}

void RequestFree(Request* req) {
	free(req->dep_id);
	free(req->text);
	free(req);
}

void RequestPrint(Request* req) {
	char* buffer = (char*)malloc(1024);
	strftime(buffer, 1024, "%Y-%m-%d %H:%M:%S", &(req->req_time_get));
	printf("Time get: %s, pr: %d, dep_id: %s, text: %s \n",
		buffer, req->priority, req->dep_id, req->text);
}

void DepFree(Department* dep, ReqStoreType type) {
	free(dep->dep_id);
	GenericFreeReqStruct(dep->req_queue, type);
	free(dep->operators);
	free(dep);
}