#define _CRT_SECURE_NO_WARNINGS
#include "Generics.h"

kErrors GenericInsertRequest(MainModel* model, Request* req, Department** found_dep) {
	kErrors status;
	Department* dep = NULL;
	switch (model->dep_store_type) {
		case DYNAMIC_ARRAY:
			dep = DArraySearch((DArray*)model->departments_storage_structure, req->dep_id);
			if (dep == NULL) {
				return NO_SUCH_EL;
			}
			break;
		case BINARY_SEARCH_TREE:
			dep = BSTSearch((BST*)model->departments_storage_structure, req->dep_id);
			if (dep == NULL) {
				return NO_SUCH_EL;
			}
	}
	switch (model->req_store_type) {
		case SKEW_HEAP:
			status = SkewHeapInsert((SkewHeapPriorityQueue*)dep->req_queue, req->priority, req);
			if (status != SUCCESS) {
				return status;
			}
			dep->requests_in_queue++;
			break;
		case LEFTIST_HEAP:
			status = LeftistHeapInsert((LeftistHeapPriorityQueue*)dep->req_queue, req->priority, req);
			if (status != SUCCESS) {
				return status;
			}
			dep->requests_in_queue++;
			break;

		case BINARY_HEAP:
			status = BinaryHeapInsert((BinaryHeapPriorityQueue*)dep->req_queue, req->priority, req);
			if (status != SUCCESS) {
				return status;
			}
			dep->requests_in_queue++;
			break;
	}
	*found_dep = dep;
	return SUCCESS;
}

void* GenericMallocReqQueue(MainModel* model) {
	switch (model->req_store_type) {
		case BINARY_HEAP:
			return malloc(sizeof(BinaryHeapPriorityQueue));
		case SKEW_HEAP:
			return malloc(sizeof(SkewHeapPriorityQueue));
		case LEFTIST_HEAP:
			return malloc(sizeof(LeftistHeapPriorityQueue));
	}
	return NULL;
}

void* GenericMallocDepStruct(MainModel* model) {
	switch (model->dep_store_type) {
		case DYNAMIC_ARRAY:
			return malloc(sizeof(DArray));
		case BINARY_SEARCH_TREE:
			return malloc(sizeof(BST));
	}
	return NULL;
}

kErrors GenericCreatePQ(void* queue, bool cmpMax(int a, int b), MainModel* model) {
	switch (model->req_store_type) {
		case SKEW_HEAP:
			return SkewHeapCreatePriorityQueue((SkewHeapPriorityQueue*)queue, cmpMax);
		case BINARY_HEAP:
			return BinaryHeapCreatePriorityQueue(10, (BinaryHeapPriorityQueue*)queue, cmpMax);
		case LEFTIST_HEAP:
			return LeftistHeapCreatePriorityQueue((LeftistHeapPriorityQueue*)queue, cmpMax);
	}
	return INC_INP_DATA;
}

kErrors GenericDeleteMax(void* req_queue, Request** out, MainModel* model) {
	switch (model->req_store_type) {
		case SKEW_HEAP:
			return SkewHeapDeleteMax((SkewHeapPriorityQueue*)req_queue, out);
		case BINARY_HEAP:
			return BinaryHeapDeleteMax((BinaryHeapPriorityQueue*)req_queue, out);
		case LEFTIST_HEAP:
			return LeftistHeapDeleteMax((LeftistHeapPriorityQueue*)req_queue, out);
	}
	return INC_INP_DATA;
}

kErrors GenericCreateDepStruct(void* structure, MainModel* model) {
	switch (model->dep_store_type) {
		case DYNAMIC_ARRAY:
			return DArrayCreate((DArray*)structure, cmpDA, cmpId, model);
		case BINARY_SEARCH_TREE:
			return BSTCreate((BST*)structure, cmpId, model);
	}
	return INC_INP_DATA;
}

kErrors GenericInsert(void* structure, Department* dep, char* id, MainModel* model){
	switch (model->dep_store_type) {
		case DYNAMIC_ARRAY:
			return DArrayInsert((DArray*)structure, dep, id);
		case BINARY_SEARCH_TREE:
			return BSTInsert((BST*)structure, dep);
	}
	return INC_INP_DATA;
}

kErrors GenericMeldReqPq(void* q_from, void* q_to, MainModel* model) {
	switch (model->req_store_type) {
		case SKEW_HEAP:
			return SkewHeapMeld((SkewHeapPriorityQueue*)q_from, (SkewHeapPriorityQueue*)q_to);
		case LEFTIST_HEAP:
			return LeftistHeapMeld((LeftistHeapPriorityQueue*)q_from, (LeftistHeapPriorityQueue*)q_to);
		case BINARY_HEAP:
			return BinaryHeapMeld((BinaryHeapPriorityQueue*)q_from, (BinaryHeapPriorityQueue*)q_to);
	}
	return INC_INP_DATA;
}

void GenericFreeReqStruct(void* st, MainModel* model) {
	switch (model->req_store_type) {
	case SKEW_HEAP:
		SkewHeapFree((SkewHeapPriorityQueue*)st);
	case LEFTIST_HEAP:
		LeftistHeapFreePriorityQueue((LeftistHeapPriorityQueue*)st);
	case BINARY_HEAP:
		BinaryHeapFreePriorityQueue((BinaryHeapPriorityQueue*)st);
	}
	return INC_INP_DATA;
}

void GenericFreeDepStruct(void* st, MainModel* model) {
	switch (model->dep_store_type) {
	case DYNAMIC_ARRAY:
		return DArrayFree((DArray*)st, model);
	case BINARY_SEARCH_TREE:
		return BSTFree((BST*)st);
	}
}

void GenericFreeAll(MainModel* model) {
	free(model->current_time_string);
	queue_destroy(model->pending_requests);
	GenericFreeDepStruct(model->departments_storage_structure, model);	
	free(model->departments_storage_array);
	//GenericFreeDepStruct(model->departments_storage_structure, model);
}