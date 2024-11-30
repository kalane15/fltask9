#include "FibonacciHeap.h"

kErrors FibHeapCreate(FibHeapPQ* pq) {
	pq->min = NULL;
	pq->size = 0;
	return SUCCESS;
}

kErrors FibHeapCreateNode(FibHeapNode** out, int key, Request* data) {
	*out = (FibHeapNode*)malloc(sizeof(FibHeapNode));
	if (*out == NULL) {
		return MEM_ALLOC_ERR;
	}
	(*out)->child = NULL;
	kErrors status = RequestCopy(&(*out)->data, data);
	if (status != SUCCESS) {
		return status;
	}

	(*out)->degree = 0;
	(*out)->left = NULL;
	(*out)->right = NULL;
	(*out)->parent = NULL;
	(*out)->key = key;
	(*out)->mark = false;
	return SUCCESS;
}

kErrors FibHeapInsert(FibHeapPQ* heap, int key, Request* data) {
	FibHeapNode* new_node = NULL;
	kErrors status = FibHeapCreateNode(&new_node, key, data);
	if (status != SUCCESS) {
		return status;
	}
	new_node->key = key;
	if (heap->size == 0) {
		heap->min = new_node;
		heap->min->left = new_node;
		heap->min->right = new_node;
	}
	else {
		FibHeapNode* prevRight = heap->min->right;
		heap->min->right = new_node;
		new_node->left = heap->min;
		new_node->right = prevRight;
		prevRight->left = new_node;
		if (new_node->key < heap->min->key) {
			heap->min = new_node;
		}
	}
	heap->size++;
	return SUCCESS;
}

Request* GetMax(FibHeapPQ* heap) {
	if (heap->min == NULL) return NULL;
	return heap->min->data;
}

kErrors FibHeapUnionLists(FibHeapNode* first, FibHeapNode* second) {
	if (first == NULL || second == NULL) {
		return SUCCESS;
	}
	FibHeapNode* l = first->left;
	FibHeapNode* r = second->right;
	second->right = first;
	first->left = second;
	l->right = r;
	r->left = l;
	return SUCCESS;
}

void FibHeapPrintTree(FibHeapNode* root) {
	if (root == NULL) {
		return;
	}
	printf("%d", root->key);
	FibHeapNode* cur = root->child;
	if (cur == NULL) {
		printf("[]");
		return;
	}
	printf("[");
	FibHeapPrintTree(cur);
	cur = cur->right;
	while (cur != root->child) {
		FibHeapPrintTree(cur);
		cur = cur->right;
	}
	printf("]");
}

void FibHeapPrint(FibHeapPQ* heap) {
	FibHeapNode* cur = heap->min->right;
	FibHeapPrintTree(heap->min);
	printf("\n");
	while (cur != heap->min) {
		FibHeapPrintTree(cur);
		printf("\n");
		cur = cur->right;
	}
}

kErrors FibHeapCopyFromRoot(FibHeapNode** dest, FibHeapNode* source, FibHeapNode* dparent) {
	FibHeapNode* prev = NULL;
	FibHeapNode* cur_s = source;
	FibHeapNode** cur_dest = dest;
	kErrors status;
	while (cur_s != source || *dest == NULL) {
		status = FibHeapCreateNode(cur_dest, cur_s->key, cur_s->data);
		if (status != SUCCESS) {
			return status;
		}
		if (cur_s->child != NULL) {
			status = FibHeapCopyFromRoot(&(*cur_dest)->child, cur_s->child, (*cur_dest));
			if (status != SUCCESS) {
				return status;
			}
		}
		(*cur_dest)->parent = dparent;
		(*cur_dest)->left = prev;
		prev = *cur_dest;
		cur_s = cur_s->right;
		cur_dest = &(*cur_dest)->right;
	}

	(*dest)->left = prev;
	prev->right = (*dest);
	return SUCCESS;
}

kErrors FibHeapCopy(FibHeapPQ* dest, FibHeapPQ* source) {
	dest->size = source->size;
	return FibHeapCopyFromRoot(&dest->min, source->min, NULL);	
}

kErrors FibHeapDeleteMax(FibHeapPQ* heap, Request** out) {
	if (heap->min == NULL) {
		return INC_ARGS;
	}
	kErrors status = RequestCopy(out, heap->min->data);
	if (status != SUCCESS) {
		return status;
	}
	FibHeapNode* prev_min = heap->min;
	FibHeapUnionLists(heap->min, heap->min->child);
	FibHeapNode* l = heap->min->left;
	FibHeapNode* r = heap->min->right;
	l->right = r;
	r->left = l;
	heap->size--;
	if (prev_min->right == prev_min) {
		heap->min = NULL;
		return SUCCESS;
	}
	heap->min = heap->min->right;
	FibHeapConsolidate(heap);	
	return SUCCESS;
}

kErrors FibHeapConsolidate(FibHeapPQ* heap) {
	if (heap == NULL || heap->min == NULL) { return INC_ARGS; }
	FibHeapNode** A = (FibHeapNode**)calloc(heap->size, sizeof(FibHeapNode*));
	if (A == NULL) {
		return MEM_ALLOC_ERR;
	}
	A[heap->min->degree] = heap->min;
	FibHeapNode* current = heap->min->right;
	bool have_changed = true;
	while (A[current->degree] != current) {
		have_changed = false;
		if (A[current->degree] == NULL) {
			A[current->degree] = current;
			current = current->right;
		}
		else {
			have_changed = true;
			FibHeapNode* conflict = A[current->degree];
			A[current->degree] = NULL;
			FibHeapNode* add_to, * adding;
			if (conflict->key < current->key) {
				add_to = conflict;
				adding = current;
			}
			else {
				add_to = current;
				adding = conflict;
			}
			if (add_to->child == NULL) {
				add_to->child = adding;
				adding->right->left = adding->left;
				adding->left->right = adding->right;
				add_to->child->left = add_to->child;
				add_to->child->right = add_to->child;
			}
			else {				
				adding->right->left = adding->left;
				adding->left->right = adding->right;
				adding->left = adding;
				adding->right = adding;
				FibHeapUnionLists(add_to->child, adding);
			}
			adding->parent = add_to;
			add_to->degree++;
			current = add_to;
		}
		if (heap->min->key > current->key) {
			heap->min = current;
		}
	}
	free(A);
	return SUCCESS;
}

kErrors FibHeapMerge(FibHeapPQ* h1, FibHeapPQ* h2,
		FibHeapPQ** merged) {
	if (h1->size == 0) {
		*merged = h1;
		return SUCCESS;
	}
	if (h2->size == 0) {
		*merged = h2;
		return SUCCESS;
	}
	kErrors status = FibHeapUnionLists(h1->min, h2->min);
	if (status != SUCCESS) {
		return status;
	}

	(*merged)->size = h1->size + h2->size;
	(*merged)->min = h1->min < h2->min ? h1->min : h2->min;
	return SUCCESS;
}

kErrors FibHeapMergeWithoutDestruction(FibHeapPQ* h1, FibHeapPQ* h2,
	FibHeapPQ** merged) {
	FibHeapPQ* tmp1 = (FibHeapPQ*)malloc(sizeof(FibHeapPQ));
	if (tmp1 == NULL) {
		return MEM_ALLOC_ERR;
	}
	FibHeapPQ* tmp2 = (FibHeapPQ*)malloc(sizeof(FibHeapPQ));
	if (tmp2 == NULL) {
		free(tmp1);
		return MEM_ALLOC_ERR;
	}
	FibHeapCreate(tmp1);
	FibHeapCreate(tmp2);
	FibHeapCopy(tmp1, h1);
	FibHeapCopy(tmp2, h2);
	return FibHeapMerge(tmp1, tmp2, merged);
}


kErrors FibHeapMergeWithDestruction(FibHeapPQ* h1, FibHeapPQ* h2,
	FibHeapPQ** merged) {
	FibHeapPQ* tmp1 = (FibHeapPQ*)malloc(sizeof(FibHeapPQ));
	if (tmp1 == NULL) {
		return MEM_ALLOC_ERR;
	}
	FibHeapPQ* tmp2 = (FibHeapPQ*)malloc(sizeof(FibHeapPQ));
	if (tmp2 == NULL) {
		free(tmp1);
		return MEM_ALLOC_ERR;
	}
	FibHeapCreate(tmp1);
	FibHeapCreate(tmp2);
	FibHeapCopy(tmp1, h1);
	FibHeapCopy(tmp2, h2);
	FibHeapFree(h1);
	FibHeapFree(h2);
	kErrors status = FibHeapMerge(tmp1, tmp2, merged);
	free(tmp1);
	free(tmp2);
	return status;
}

void FibHeapFreeRecursive(FibHeapNode* node) {
	if (node == NULL) {
		return;
	}
	
	FibHeapNode* cur_s = node->right;
	FibHeapNode* tmp;
	FibHeapFreeRecursive(node->child);
	while (cur_s != node) {		
		FibHeapFreeRecursive(cur_s->child);
		tmp = cur_s;
		cur_s = cur_s->right;
		free(tmp);
	}
	
	free(node);
	return SUCCESS;
}

void FibHeapFree(FibHeapPQ* heap) {
	FibHeapFreeRecursive(heap->min);
	free(heap);
}

kErrors FibHeapMeld(FibHeapPQ* p_in, FibHeapPQ* p_out) {
	kErrors status = SUCCESS;
	FibHeapNode* cur = NULL;
	Request* cur_req = NULL;
	while (p_in->size > 0) {
		status = FibHeapDeleteMax(p_in, &cur_req);
		if (status != SUCCESS) {
			break;
		}
		status = FibHeapInsert(p_out, cur_req->priority, cur_req);
		if (status != SUCCESS) {
			return status;
		}
	}
	return SUCCESS;
}