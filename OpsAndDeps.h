#pragma once
typedef enum ReqStoreType ReqStoreType;
typedef enum DepStoreType DepStoreType;

#define TYPESH Request
typedef struct queue_req queue_req;
#include "time.h"
#include "queue_req.h"
#include "InputParsing.h"

typedef struct MainModel {
	void* departments_storage_structure;
	Department** departments_storage_array;
	int min_op_work;
	int max_op_work;
	int max_priority;
	queue_req* pending_requests;
	time_t modelling_start;
	time_t modelling_finished;
	time_t current_time;
	char* current_time_string;
	int deps_count;
	ReqStoreType req_store_type;
	DepStoreType dep_store_type;
} MainModel;

typedef struct Request {
	struct tm req_time_get;
	int priority;
	char* dep_id;
	char* text;
	int r_id;
	int time_to_operate;
} Request;

typedef struct Operator {
	int operating_time_remain;
	struct Request* current_request;
	bool is_working;
} Operator;

typedef struct Department {
	void* req_queue;
	struct Operator* operators;
	int op_count;
	int overload_coeff;
	int requests_in_queue;
	char* dep_id;
} Department;

kErrors DepCreate(int op_count, int overload_coeff, Department** out, char* id, MainModel* model);
kErrors RequestCreate(int priority, char* id, char* text, char* time, Request** out, int r_id, MainModel* model);
kErrors RequestCopy(Request** dest, Request* source);
void RequestFree(Request* req);
void RequestPrint(Request* req);

void DepFree(Department* dep, ReqStoreType model);
