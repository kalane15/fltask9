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
			break;
		case HASH_SET:
			dep = HashTableSearch((HashTable*)model->departments_storage_structure, req->dep_id);
			if (dep == NULL) {
				return NO_SUCH_EL;
			}
			break;
		case TRIE:
			dep = TrieSearch((Trie*)model->departments_storage_structure, req->dep_id);
			if (dep == NULL) {
				return NO_SUCH_EL;
			}
			break;
	}
	switch (model->req_store_type) {
		case SKEW_HEAP:
			status = SkewHeapInsert((SkewHeapPQ*)dep->req_queue, req->priority, req);
			if (status != SUCCESS) {
				return status;
			}
			dep->requests_in_queue++;
			break;
		case LEFTIST_HEAP:
			status = LeftistHeapInsert((LeftistHeapPQ*)dep->req_queue, req->priority, req);
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
		case BINOMIAL_HEAP:
			status = BinomialHeapInsert((BinomialHeapPQ*)dep->req_queue, req->priority, req);
			if (status != SUCCESS) {
				return status;
			}
			dep->requests_in_queue++;
			break;
		case TREAP:
			status = TreapInsert((Treap*)dep->req_queue, req->priority, req);
			if (status != SUCCESS) {
				return status;
			}
			dep->requests_in_queue++;
			break;
		case FIB_HEAP:
			status = FibHeapInsert((FibHeapPQ*)dep->req_queue, req->priority, req);
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
			return malloc(sizeof(SkewHeapPQ));
		case LEFTIST_HEAP:
			return malloc(sizeof(LeftistHeapPQ));
		case BINOMIAL_HEAP:
			return malloc(sizeof(BinomialHeapPQ));
		case TREAP:
			return malloc(sizeof(Treap));
		case FIB_HEAP:
			return malloc(sizeof(FibHeapPQ));
	}
	return NULL;
}

void* GenericMallocDepStruct(MainModel* model) {
	switch (model->dep_store_type) {
		case DYNAMIC_ARRAY:
			return malloc(sizeof(DArray));
		case BINARY_SEARCH_TREE:
			return malloc(sizeof(BST));
		case HASH_SET:
			return malloc(sizeof(HashTable));
		case TRIE:
			return malloc(sizeof(Trie));
	}
	return NULL;
}

kErrors GenericCreatePQ(void* queue, bool cmpMax(int a, int b), MainModel* model) {
	switch (model->req_store_type) {
		case SKEW_HEAP:
			return SkewHeapCreatePriorityQueue((SkewHeapPQ*)queue, cmpMax);
		case BINARY_HEAP:
			return BinaryHeapCreatePriorityQueue(10, (BinaryHeapPriorityQueue*)queue, cmpMax);
		case LEFTIST_HEAP:
			return LeftistHeapCreatePriorityQueue((LeftistHeapPQ*)queue, cmpMax);
		case BINOMIAL_HEAP:
			return BinomialHeapCreatePriorityQueue((BinomialHeapPQ*)queue, cmpMax);
		case TREAP:
			return TreapCreate((Treap*)queue);
		case FIB_HEAP:
			return FibHeapCreate((FibHeapPQ*)queue);
	}
	return INC_INP_DATA;
}

kErrors GenericDeleteMax(void* req_queue, Request** out, MainModel* model) {
	switch (model->req_store_type) {
		case SKEW_HEAP:
			return SkewHeapDeleteMax((SkewHeapPQ*)req_queue, out);
		case BINARY_HEAP:
			return BinaryHeapDeleteMax((BinaryHeapPriorityQueue*)req_queue, out);
		case LEFTIST_HEAP:
			return LeftistHeapDeleteMax((LeftistHeapPQ*)req_queue, out);
		case BINOMIAL_HEAP:
			return BinomialHeapDeleteMax((BinomialHeapPQ*)req_queue, out);
		case TREAP:
			return TreapDeleteMax((Treap*)req_queue, out);
		case FIB_HEAP:
			return FibHeapDeleteMax((FibHeapPQ*)req_queue, out);
	}
	return INC_INP_DATA;
}

kErrors GenericCreateDepStruct(void* structure, MainModel* model) {
	switch (model->dep_store_type) {
		case DYNAMIC_ARRAY:
			return DArrayCreate((DArray*)structure, cmpDA, cmpId, model);
		case BINARY_SEARCH_TREE:
			return BSTCreate((BST*)structure, cmpId, model);
		case HASH_SET:
			return HashTableCreate((HashTable*)structure, model->deps_count);
		case TRIE:
			return TrieCreate((Trie*)structure);
	}
	return INC_INP_DATA;
}

kErrors GenericInsert(void* structure, Department* dep, char* id, MainModel* model){
	switch (model->dep_store_type) {
		case DYNAMIC_ARRAY:
			return DArrayInsert((DArray*)structure, dep, id);
		case BINARY_SEARCH_TREE:
			return BSTInsert((BST*)structure, dep);
		case HASH_SET:
			return HashTableInsert((HashTable*)structure, id, dep);
		case TRIE:
			return TrieInsert((Trie*)structure, id, dep);
	}
	return INC_INP_DATA;
}

kErrors GenericMeldReqPq(void* q_from, void* q_to, MainModel* model) {
	switch (model->req_store_type) {
		case SKEW_HEAP:
			return SkewHeapMeld((SkewHeapPQ*)q_from, (SkewHeapPQ*)q_to);
		case LEFTIST_HEAP:
			return LeftistHeapMeld((LeftistHeapPQ*)q_from, (LeftistHeapPQ*)q_to);
		case BINARY_HEAP:
			return BinaryHeapMeld((BinaryHeapPriorityQueue*)q_from, (BinaryHeapPriorityQueue*)q_to);
		case BINOMIAL_HEAP:
			return BinomialHeapMeld((BinomialHeapPQ*)q_from, (BinomialHeapPQ*)q_to);
		case TREAP:
			return TreapMeld((Treap*)q_from, (Treap*)q_to);
		case FIB_HEAP:
			return FibHeapMeld((FibHeapPQ*)q_from, (FibHeapPQ*)q_to);
	}
	return INC_INP_DATA;
}

void GenericFreeReqStruct(void* st, ReqStoreType type) {
	switch (type) {
	case SKEW_HEAP:
		SkewHeapFree((SkewHeapPQ*)st);
		break;
	case LEFTIST_HEAP:
		LeftistHeapFreePriorityQueue((LeftistHeapPQ*)st);
		break;
	case BINARY_HEAP:
		BinaryHeapFreePriorityQueue((BinaryHeapPriorityQueue*)st);
		break;
	case BINOMIAL_HEAP:
		BinomialHeapFree((BinomialHeapPQ*)st);
		break;
	case TREAP:
		TreapFree((Treap*)st);
		break;
	case FIB_HEAP:
		FibHeapFree((FibHeapPQ*)st);
		break;
	}
	return;
}

void GenericFreeDepStruct(void* st, MainModel* model) {
	switch (model->dep_store_type) {
	case DYNAMIC_ARRAY:
		return DArrayFree((DArray*)st, model);
	case BINARY_SEARCH_TREE:
		return BSTFree((BST*)st);
	case HASH_SET:
		return HashTableFree((HashTable*)st);
	case TRIE:
		return TrieFree((Trie*)st);
	}
}

void GenericFreeAll(MainModel* model) {
	free(model->current_time_string);
	queue_destroy(model->pending_requests);
	GenericFreeDepStruct(model->departments_storage_structure, model);	
	free(model->departments_storage_array);
	//GenericFreeDepStruct(model->departments_storage_structure, model);
}