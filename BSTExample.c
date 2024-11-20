#define _CRT_SECURE_NO_WARNINGS
#include "LeftistHeap.h"
#include "myerrors.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "BSTExample.h"

kErrors BSTCreate(BST* tree, int (*_cmp)(char*, char*), MainModel* model) {
	tree->cmpLess = _cmp;
	tree->root = NULL;
	tree->model = model;
	return SUCCESS;
}

BSTNode* BSTCreateNode(Department* data) {
	BSTNode* res = (BSTNode*)malloc(sizeof(BSTNode));
	if (res == NULL) {
		return NULL;
	}
	res->data = data;
	res->left = NULL;
	res->right = NULL;

	if (res->data->dep_id == NULL) {
		free(res);
		return NULL;
	}
	return res;
}

kErrors BSTInsert(BST* tree, Department* data) {
	BSTNode** current = &tree->root;
	BSTNode* new_node = BSTCreateNode(data);

	if (new_node == NULL) {
		return MEM_ALLOC_ERR;
	}
	while (*current != NULL) {
		if (tree->cmpLess(new_node->data->dep_id, (*current)->data->dep_id) < 0) {
			current = &(*current)->left;
		}
		else if (tree->cmpLess(new_node->data->dep_id, (*current)->data->dep_id) > 0) {
			current = &(*current)->right;
		}
		else {
			return EL_EXIST;
		}
	}
	*current = new_node;

	return SUCCESS;
}

void BSTFreeNode(BSTNode* node, MainModel* model) {
	DepFree(node->data, model->req_store_type);
	free(node);
}

kErrors BSTDelete(BST* tree, char* id) {
	BSTNode** to_delete = &tree->root;
	BSTNode* prev;
	while (tree->cmpLess((*to_delete)->data->dep_id, id) != 0) {
		if (tree->cmpLess(id, (*to_delete)->data->dep_id) < 0) {
			to_delete = &(*to_delete)->left;
		}
		else {
			to_delete = &(*to_delete)->right;
		}
		prev = *to_delete;
		if (to_delete == NULL) {
			return NO_SUCH_EL;
		}
	}

	BSTNode* res;
	BSTNode* prev_res;
	if ((*to_delete)->left != NULL) {
		res = (*to_delete)->left;
		prev_res = (*to_delete);
		while (res->right != NULL) {
			prev_res = res;
			res = res->right;
		}	
		
		if (prev_res == (*to_delete)) {
			res->right = (*to_delete)->right;
			BSTFreeNode(*to_delete, tree->model);
			*to_delete = res;
		}
		else {
			prev_res->right = NULL;
			res->right = (*to_delete)->right;
			res->left = (*to_delete)->left;
			BSTFreeNode(*to_delete, tree->model);
			*to_delete = res;
		}				
	}
	else if ((*to_delete)->right != NULL){
		res = (*to_delete)->right;
		prev_res = (*to_delete);
		while (res->left != NULL) {
			prev_res = res;
			res = res->left;
		}

		if (prev_res == (*to_delete)) {
			res->left = (*to_delete)->left;
			BSTFreeNode(*to_delete, tree->model);
			*to_delete = res;
		}
		else {
			prev_res->left = NULL;
			res->left = (*to_delete)->left;
			res->right = (*to_delete)->right;
			BSTFreeNode(*to_delete, tree->model);
			*to_delete = res;
		}
	}
	else {

	}

	return SUCCESS;
}

void BSTPrintNode(BSTNode* root) {
	printf("%s \n", root->data->dep_id);
}

void BSTPrintNodes(BSTNode* root, int tab_count) {
	if (root != NULL)
	{
		BSTPrintNodes(root->right, tab_count + 1);
		for (int i = 0; i < tab_count; ++i) { printf("\t"); }
		BSTPrintNode(root);
		BSTPrintNodes(root->left, tab_count + 1);
	}
}

void BSTPrint(BST* p)
{
	BSTPrintNodes(p->root, 0);
}

Department* BSTSearch(BST* tree, char* id) {
	BSTNode* current = tree->root;
	while (current != NULL) {
		if (tree->cmpLess(id, current->data->dep_id) < 0) {
			current = current->left;
		}
		else if (tree->cmpLess(id, current->data->dep_id) > 0) {
			current = current->right;
		}
		else {
			break;
		}
	}
	if (current == NULL) {
		return NULL;
	}
	return current->data;
}

void BSTFreeTreeRecursive(BSTNode* node, MainModel* model) {
	if (node == NULL) {
		return;
	}
	BSTFreeTreeRecursive(node->left, model);
	BSTFreeTreeRecursive(node->right, model);
	BSTFreeNode(node, model);
}

void BSTFree(BST* tree) {
	BSTFreeTreeRecursive(tree->root, tree->model);
	free(tree);
}
