#include "Treap.h"

kErrors TreapCreate(Treap* t){
	t->size = 0;
	t->root = NULL;
	return SUCCESS;
}

kErrors TreapSplit(Treap* to_split, int key, Treap** l, Treap** r) {
	TreapSplitRecursive(to_split->root, key, &(*l)->root, &(*r)->root);
	return SUCCESS;
}

kErrors TreapCreateNode(TreapNode** out, int priority, Request* data) {
	*out = (TreapNode*)malloc(sizeof(TreapNode));
	if (*out == NULL) {
		return MEM_ALLOC_ERR;
	}
	(*out)->left = NULL;
	(*out)->right = NULL;
	(*out)->x = 1;
	(*out)->y = priority;
	(*out)->data = data;
	return SUCCESS;
}

int GetX(TreapNode* t) { return t == NULL ? 0 : t->x; }

void TreapSplitRecursive(TreapNode* t, int x0, TreapNode** l, TreapNode** r) {
	if (t == NULL) {
		*l = NULL;
		*r = NULL;
		return;
	}
	if (x0 >= 1 + GetX(t->left)) {
		TreapSplitRecursive(t->right, x0 - (1 + GetX(t->left)), &(t->right), r);
		*l = t;
		
	}
	else {	
		TreapSplitRecursive(t->left, x0, l, &(t->left));
		*r = t;
	}
	TreapNodeUpdate(t);
}

void TreapNodeUpdate(TreapNode* t) {
	if (t != NULL) {
		t->x = 1 + GetX(t->left) + GetX(t->right);
	}
}

TreapNode* TreapMergeRecursive(TreapNode* l, TreapNode* r) {
	if (l == NULL) {
		return r;
	}
	if (r == NULL) {
		return l;
	}
	if (l->y > r->y) {
		l->right = TreapMergeRecursive(l->right, r);
		TreapNodeUpdate(l);
		return l;
	}
	else {
		r->left = TreapMergeRecursive(l, r->left);
		TreapNodeUpdate(r);
		return r;
	}
}
kErrors TreapCopyNodes(TreapNode** dest, TreapNode* source) {
	kErrors status = SUCCESS;
	if (source == NULL) {
		return SUCCESS;
	}

	if (*dest != NULL) {
		TreapFreeRecursive(*dest);
	}

	*dest = (TreapNode*)malloc(sizeof(TreapNode));
	if ((*dest) == NULL) {
		return MEM_ALLOC_ERR;
	}
	(*dest)->left = NULL;
	(*dest)->right = NULL;
	(*dest)->x = source->x;
	(*dest)->y = source->y;
	Request* new_request;
	status = RequestCopy(&new_request, source->data);
	if (status != SUCCESS) {
		return status;
	}

	(*dest)->data = new_request;
	status = TreapCopyNodes(&((*dest)->left), source->left);
	if (status == SUCCESS) {
		status = TreapCopyNodes(&((*dest)->right), source->right);
	}
	return status;
}

kErrors TreapCopy(Treap* dest, Treap* source) {
	if (source == NULL) {
		return SUCCESS;
	}
	return TreapCopyNodes(&(dest->root), source->root);
}

kErrors TreapMergeWithoutDestruction(Treap* t1, Treap* t2, Treap** merged) {
	Treap* tmp1 = (Treap*)malloc(sizeof(Treap));
	if (tmp1 == NULL) {
		return MEM_ALLOC_ERR;
	}
	Treap* tmp2 = (Treap*)malloc(sizeof(Treap));
	if (tmp2 == NULL) {
		free(tmp1);
		return MEM_ALLOC_ERR;
	}
	TreapCreate(tmp1);
	TreapCreate(tmp2);
	TreapCopy(tmp1, t1);
	TreapCopy(tmp2, t2);
	*merged = (Treap*)malloc(sizeof(Treap));
	if (*merged == NULL) {
		return MEM_ALLOC_ERR;
	}
	TreapCreate(*merged);	
	(*merged)->root = TreapMergeRecursive(tmp1->root, tmp2->root);
	free(tmp1);
	free(tmp2);
}

kErrors TreapMergeWithDestruction(Treap* t1, Treap* t2, Treap** merged) {
	kErrors status = TreapMergeWithoutDestruction(t1, t2, merged);
	if (status != SUCCESS) {
		return status;
	}
	TreapFree(t1);
	TreapFree(t2);
}

kErrors TreapInsertToRoot(TreapNode** root, int k, Request* data) {
	TreapNode* l = NULL;
	TreapNode* r = NULL;
	TreapSplitRecursive(*root, k, &l, &r);
	TreapNode* m = NULL;
	TreapCreateNode(&m, k, data);
	*root = TreapMergeRecursive(TreapMergeRecursive(l, m), r);
	return SUCCESS;
}

kErrors TreapInsert(Treap* treap, int p, Request* data) {
	TreapInsertToRoot(&treap->root, p, data);
	treap->size++;
	return SUCCESS;
}

void TreapFree(Treap* treap) {
	TreapFreeRecursive(treap->root);
	free(treap);
}

void TreapFreeRecursive(TreapNode* node) {
	if (node == NULL) {
		return;
	}
	TreapFreeRecursive(node->left);
	TreapFreeRecursive(node->right);
	free(node);
}


kErrors TreapDeleteMax(Treap* treap, Request** out) {
	if (treap->root == NULL) {
		return INC_ARGS;
	}
	RequestCopy(out, treap->root->data);
	TreapNode* tmp = treap->root;
	treap->root = TreapMergeRecursive(treap->root->left, treap->root->right);
	free(tmp);
	treap->size--;
	return SUCCESS;
}

kErrors TreapMeld(Treap* p_in, Treap* p_out) {
	kErrors status = SUCCESS;
	TreapNode* cur = NULL;
	Request* cur_req = NULL;
	while (p_in->size > 0) {
		status = TreapDeleteMax(p_in, &cur_req);
		if (status != SUCCESS) {
			return status;
		}
		status = TreapInsert(p_out, cur_req->priority, cur_req);
		if (status != SUCCESS) {
			return status;
		}
	}
	return SUCCESS;
}

void TreapPrintNode(TreapNode* root) {
	printf("%d\n", root->y);
}


void TreapPrintNodes(TreapNode* root, int tab_count) {
	if (root != NULL)
	{
		TreapPrintNodes(root->right, tab_count + 1);
		for (int i = 0; i < tab_count; ++i) { printf("\t"); }
		TreapPrintNode(root);
		TreapPrintNodes(root->left, tab_count + 1);
	}
}

void TreapPrint(Treap* p)
{
	TreapPrintNodes(p->root, 0);
}

Request* TreapGetMax(Treap* heap) {
	return heap->root->data;
}