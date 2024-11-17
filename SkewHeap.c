#include <stdio.h>
#include <stdlib.h>
#include "myerrors.h"
#include <stdbool.h>
#include "SkewHeap.h"
// Вспомогательная функция для создания нового узла
SkewHeapNode* SkewHeapCreateNode(int key, TYPESH* data) {
    SkewHeapNode* node = (SkewHeapNode*)malloc(sizeof(SkewHeapNode));
    if (!node) {
        return NULL; // Ошибка выделения памяти
    }
    kErrors status = RequestCopy(&node->data, data);
    if (status != SUCCESS) {
        return NULL;
    }
    node->left = NULL;
    node->right = NULL;
    return node;
}
 
kErrors SkewHeapMergeWithoutDestruction(SkewHeapPriorityQueue* pq1, SkewHeapPriorityQueue* pq2, SkewHeapPriorityQueue* merged) {
    kErrors status = SkewHeapMergeByRootsWithCopy(pq1->root, pq2->root, &(merged->root), pq1->cmp);
    if (status != SUCCESS) {
        return status;
    }
    merged->size = pq1->size + pq2->size;
    return SUCCESS;
}

// Функция слияния двух косых куч собирает новую кучу при этом ноды везде одни и те же
kErrors SkewHeapMergeByRootsWithCopy(SkewHeapNode* h1, SkewHeapNode* h2, SkewHeapNode** out, bool(*cmp)(SkewHeapNode*, SkewHeapNode*)) {
    kErrors status = SUCCESS;
    if (!h1) {
        return SkewHeapCopy(out, h2);
    }
    if (!h2) {
        return SkewHeapCopy(out, h1);
    }
    if (*out == NULL) {
        *out = (SkewHeapNode*)malloc(sizeof(SkewHeapNode));
        if (*out == NULL) {
            return MEM_ALLOC_ERR;
        }
        (*out)->right = NULL;
        (*out)->left = NULL;
        (*out)->data = NULL;
    }

    if (cmp(h1, h2)) {
        SkewHeapNode* temp = NULL;
        temp = h1->right;
        status = SkewHeapCopy(&(*out)->right, h1->left);
        if (status != SUCCESS) {
            return status;
        }
        SkewHeapNode* res = NULL;
        RequestCopy(&(*out)->data, h1->data);
        return  SkewHeapMergeByRootsWithCopy(h2, temp, &((*out)->left), cmp);
    }
    else {
        return SkewHeapMergeByRootsWithCopy(h2, h1, out, cmp);
    }   
    return  SkewHeapCopy(out, h1);
}

// Создание пустой очереди
kErrors SkewHeapCreatePriorityQueue(SkewHeapPriorityQueue* out, bool (*inp_cmp)(SkewHeapNode*, SkewHeapNode*)) {
    out->cmp = inp_cmp;
    out->root = NULL;
    out->size = 0;
    return SUCCESS;
}

// Добавление элемента в косую кучу
kErrors SkewHeapInsert(SkewHeapPriorityQueue* heap, int key, TYPESH* data) {
    if (!heap) return INC_ARGS; // Пустая очередь

    SkewHeapNode* newNode = SkewHeapCreateNode(key, data);
    if (!newNode) return MEM_ALLOC_ERR; // Ошибка выделения памяти

    SkewHeapNode* tmp = NULL;
    SkewHeapMergeByRootsWithCopy(heap->root, newNode, &tmp, heap->cmp);
    SkewHeapFreeRecursively(heap->root);
    heap->root = tmp;

    SkewHeapFreeRecursively(newNode);
    heap->size++;
    return SUCCESS;
}

// Удаление максимального элемента (по ключу)
kErrors SkewHeapDeleteMax(SkewHeapPriorityQueue* heap, Request** data) {
    if (!heap || !heap->root) return INC_ARGS; // Пустая очередь
    kErrors status = SUCCESS;
    SkewHeapNode* root = heap->root;
    *data = root->data;
    SkewHeapNode* temp = NULL;
    // Слияние левого и правого поддеревьев
    status = SkewHeapMergeByRootsWithCopy(heap->root->left, heap->root->right, &temp, heap->cmp);
    if (status != SUCCESS) {
        return status;
    }
    heap->root = temp;
    SkewHeapFreeRecursively(root);
    heap->size--;
    return SUCCESS;
}

kErrors SkewHeapDeleteMaxOutNode(SkewHeapPriorityQueue* heap, SkewHeapNode** data) {
    if (!heap || !heap->root) return INC_ARGS; // Пустая очередь
    kErrors status = SUCCESS;
    SkewHeapNode* root = heap->root;
    SkewHeapCopy(data, root);
    SkewHeapNode* temp = NULL;
    // Слияние левого и правого поддеревьев
    status = SkewHeapMergeByRootsWithCopy(heap->root->left, heap->root->right, &temp, heap->cmp);
    if (status != SUCCESS) {
        return status;
    }
    heap->root = temp;
    SkewHeapFreeRecursively(root);
    heap->size--;
    return SUCCESS;
}

// Поиск максимального элемента (по ключу)
kErrors SkewHeapGetMax(SkewHeapPriorityQueue* heap, SkewHeapNode* data) {
    if (!heap || !heap->root) return INC_ARGS; // Пустая очередь

    *data = *(heap->root);
    return SUCCESS;
}


 //Слияние двух очередей с разрушением исходных
kErrors SkewHeapMergeWithDestruction(SkewHeapPriorityQueue* heap1, SkewHeapPriorityQueue* heap2, SkewHeapPriorityQueue* merged) {
    if (!heap1 || !heap2) return INC_ARGS;
    kErrors status = SUCCESS;
    if (status != SUCCESS) {
        return status;
    }
    status = SkewHeapMergeWithoutDestruction(heap1, heap2, merged);
    if (status != SUCCESS) {
        return status;
    }
    merged->size = heap1->size + heap2->size;
    SkewHeapFree(heap1);
    SkewHeapFree(heap2);    
    return SUCCESS;
}

void SkewHeapFreeRecursively(SkewHeapNode* node) {
    if (node == NULL) {
        return;
    }
    SkewHeapFreeRecursively(node -> left);
    SkewHeapFreeRecursively(node -> right);
    free(node);
}

void SkewHeapFree(SkewHeapPriorityQueue* heap) {
    if (!heap) return;
    SkewHeapFreeRecursively(heap->root);
}

kErrors SkewHeapCopy(SkewHeapNode** dest, SkewHeapNode* source) {
    kErrors status = SUCCESS;
    if (source == NULL) {
        return SUCCESS;
    }

    if (*dest != NULL) {
        SkewHeapFreeRecursively(*dest);
    }

    *dest = (SkewHeapNode*)malloc(sizeof(SkewHeapNode));
    if ((*dest) == NULL) {
        return MEM_ALLOC_ERR;
    }
    (*dest)->left = NULL;
    (*dest)->right = NULL;
    Request* new_request;
    status = RequestCopy(&new_request, source->data);
    if (status != SUCCESS) {
        return status;
    }
    (*dest)->data = new_request;
    status = SkewHeapCopy(&((*dest)->left), source->left);
    if (status == SUCCESS) {
        status = SkewHeapCopy(&((*dest)->right), source->right);
    }   
    return status;
}

kErrors SkewHeapCopyPqFromRoot(SkewHeapPriorityQueue* dest, SkewHeapNode* source) {
    if (source == NULL) {
        return SUCCESS;
    }
    return SkewHeapCopy(&(dest->root), source);
}

void SkewHeapPrintNode(SkewHeapNode* node) {
    char* buffer = (char*)malloc(1024);
    strftime(buffer, 1024, "%Y-%m-%d %H:%M:%S", &(node->data->req_time_get));
    printf("Time get: %s, pr: %d, dep_id: %s, text: %s \n", 
        buffer, node->data->priority, node->data->dep_id, node->data->text);
}

void SkewHeapPrintNodes(SkewHeapNode* root, int tab_count) {
    if (root != NULL)
    {
        SkewHeapPrintNodes(root->right, tab_count + 1);
        for (int i = 0; i < tab_count; ++i) { printf("\t"); }
        SkewHeapPrintNode(root);
        SkewHeapPrintNodes(root->left, tab_count + 1);
    }
}

void SkewHeapPrint(SkewHeapPriorityQueue* p)
{
    SkewHeapPrintNodes(p->root, 0);
}

kErrors SkewHeapMeld(SkewHeapPriorityQueue* p_in, SkewHeapPriorityQueue* p_out) {
    kErrors status = SUCCESS;
    SkewHeapNode* cur = NULL;
    Request* cur_req = NULL;
    while (p_in->size > 0) {
        status = SkewHeapDeleteMax(p_in, &cur_req);
        if (status != SUCCESS) {
            return status;
        }
        status = SkewHeapInsert(p_out, cur_req->priority, cur_req);
        if (status != SUCCESS) {
            return status;
        }
    }
    return SUCCESS;
}