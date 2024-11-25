#include <stdio.h>
#include <stdlib.h>
#include "myerrors.h"
#include <stdbool.h>
#include "BinomialHeap.h"
#define BINOMIAL_BUF_SIZE 10

// Вспомогательная функция для создания нового узла
BinomialHeapNode* BinomialHeapCreateNode(int key, Request* data) {
    BinomialHeapNode* node = (BinomialHeapNode*)malloc(sizeof(BinomialHeapNode));
    if (!node) {
        return NULL; // Ошибка выделения памяти
    }
    kErrors status = RequestCopy(&node->data, data);
    if (status != SUCCESS) {
        return NULL;
    }
    node->children = (BinomialHeapNode**)calloc(BINOMIAL_BUF_SIZE, sizeof(BinomialHeapNode*));
    if (node->children == NULL) {
        free(node);
        return NULL;
    }
    node->children_size = 0;
    node->children_capacity = BINOMIAL_BUF_SIZE;
    node->key = key;
    return node;
}
//
//kErrors SkewHeapMergeWithoutDestruction(SkewHeapPriorityQueue* pq1, SkewHeapPriorityQueue* pq2, SkewHeapPriorityQueue* merged) {
//    kErrors status = SkewHeapMergeByRootsWithCopy(pq1->root, pq2->root, &(merged->root), pq1->cmp);
//    if (status != SUCCESS) {
//        return status;
//    }
//    merged->size = pq1->size + pq2->size;
//    return SUCCESS;
//}
//
//// Функция слияния двух косых куч собирает новую кучу при этом ноды везде одни и те же

kErrors BinomialHeapCreateTree(BinomialTree** out, BinomialHeapNode* node) {
    *out = (BinomialTree*)malloc(sizeof(BinomialTree));
    if (*out == NULL) {
        return MEM_ALLOC_ERR;
    }
    (*out)->degree = node != NULL ? node->children_size : 0;
    (*out)->head = node;
    (*out)->sibling = NULL;
    return SUCCESS;
}

kErrors BinomialHeapMergeByRootsWithCopy(BinomialTree* h1, BinomialTree* h2, BinomialTree** out, bool(*cmp)(int, int)) {
    /*if (*out == NULL) {
        *out = (BinomialTree*)malloc(sizeof(BinomialTree));
    }*/
   
    if (h1 == NULL) {
        BinomialHeapCopyFromTree(out, h2);
        return SUCCESS;;
    }
    if (h2 == NULL) {
        BinomialHeapCopyFromTree(out, h1);
        return SUCCESS;
    }
    
    BinomialTree* curH1 = h1;
    BinomialTree* curH2 = h2;
    BinomialTree** curH = &((*out));
    while (curH1 != NULL && curH2 != NULL) {
        if (cmp(curH1->head->key, curH2->head->key)) {
            if (*curH == NULL) {
                BinomialHeapCopyOneTree(curH, curH1);
            }
            else {
                BinomialHeapCopyOneTree(&((*curH)->sibling), curH2);
            }
            curH1 = curH1->sibling;
        }
        else {
            if (*curH == NULL) {
                BinomialHeapCopyOneTree(curH, curH2);
            }
            else {
                BinomialHeapCopyOneTree(&((*curH)->sibling), curH2);
            }
            curH2 = curH2->sibling;
        }
        curH = &((*curH)->sibling);
    }

    if (curH1 != NULL) {
        while (curH1 != NULL) {
            if (*curH == NULL) {
                BinomialHeapCopyOneTree(curH, curH1);
            }
            else {
                BinomialHeapCopyOneTree(&((*curH)->sibling), curH1);
            }
            curH1 = curH1->sibling;
        }
    }
    else {
        while (curH2 != NULL) {
            if (*curH == NULL) {
                BinomialHeapCopyOneTree(curH, curH2);
            }
            else {
                BinomialHeapCopyOneTree(&((*curH)->sibling), curH2);
            }
            curH2 = curH2->sibling;
        }
    }



   
    bool flag = true;
    while (flag) {
        curH = out;
        flag = false;
        while ((*curH)->sibling != NULL) {
            if ((*curH)->degree == (*curH)->sibling->degree) {
                flag = true;
                if ((*curH)->head->children_size == (*curH)->head->children_capacity) {
                    BinomialHeapNode** tmp = (*curH)->head->children;
                    int new_capacity = (*curH)->head->children_capacity * 2;
                    (*curH)->head->children = (BinomialHeapNode**)realloc((*curH)->head->children, new_capacity);
                    if ((*curH)->head->children == NULL) {
                        (*curH)->head->children = tmp;
                        return MEM_ALLOC_ERR;
                    }
                    (*curH)->head->children_capacity = new_capacity;
                }
                (*curH)->head->children[(*curH)->head->children_size++] = (*curH)->sibling->head;
                BinomialTree* tmp = (*curH)->sibling;
                (*curH)->sibling = (*curH)->sibling->sibling;
                (*curH)->degree = (*curH)->head->children_size;
                free(tmp);
            }
            else {
                curH = &(*curH)->sibling;
            }
        }
    }
}


//
//// Создание пустой очереди
kErrors BinomialHeapCreatePriorityQueue(BinomialHeapPQ* out, bool (*inp_cmp)(int*, int*)) {
    out->cmp = inp_cmp;
    out->root = NULL;
    out->size = 0;
    return SUCCESS;
}

kErrors BinomialHeapInsert(BinomialHeapPQ* heap, int key, Request* data) {
    if (!heap) return INC_ARGS; 

    BinomialHeapNode* newNode = BinomialHeapCreateNode(key, data);
    if (!newNode) return MEM_ALLOC_ERR;

    BinomialTree* tree = (BinomialTree*)malloc(sizeof(BinomialTree));
    if (tree == NULL) {
        free(newNode);
        return MEM_ALLOC_ERR;
    }
    tree->degree = 0;
    tree->head = newNode;
    tree->sibling = NULL;

    BinomialTree* tmp = NULL;
    BinomialHeapMergeByRootsWithCopy(heap->root, tree, &tmp, heap->cmp);
    BinomialHeapFreeFromTree(heap->root);
    heap->root = tmp;

    BinomialHeapFreeFromTree(tree);
    heap->size++;
    return SUCCESS;
}

kErrors BinomialHeapDeleteMax(BinomialHeapPQ* heap, Request** data) {
    if (!heap || !heap->root) return INC_ARGS; 
    kErrors status = SUCCESS;
    BinomialTree* min_root_tree = heap->root;
    RequestCopy(data, heap->root->head->data);
    heap->root = heap->root->sibling;

    BinomialHeapNode* min; 
    BinomialHeapNode* max;
    BinomialTree* new_tree;
    BinomialTree* tmp = NULL;
    if (min_root_tree->head->children_size >= 2) {
        if (heap->cmp(min_root_tree->head->children[0]->key, min_root_tree->head->children[1]->key)) {
            min = min_root_tree->head->children[0];
            max = min_root_tree->head->children[1];
        }
        else {
            min = min_root_tree->head->children[1];
            max = min_root_tree->head->children[0];
        }        
        BinomialHeapCreateTree(&new_tree, max);
        RequestFree(min_root_tree->head->data, BINOMIAL_HEAP);
        free(min_root_tree->head->children);
        free(min_root_tree->head);        
        min_root_tree->head = min;
        new_tree->sibling = min_root_tree->sibling == NULL ? NULL : min_root_tree->sibling->sibling;
        min_root_tree->sibling = new_tree;
        min_root_tree->degree = min_root_tree->head->children_size;
        BinomialHeapMergeByRootsWithCopy(min_root_tree, heap->root, &tmp, heap->cmp);
        BinomialHeapFreeFromTree(heap->root);
        BinomialHeapFreeFromTree(min_root_tree);
        heap->root = tmp;
    }
    else if (min_root_tree->head->children_size == 1) {
        min = min_root_tree->head->children[0];
        RequestFree(min_root_tree->head->data, BINOMIAL_HEAP);
        free(min_root_tree->head->children);
        free(min_root_tree->head);
        min_root_tree->head = min;
        min_root_tree->degree = min_root_tree->head->children_size;
        min_root_tree->sibling = NULL;
        BinomialHeapMergeByRootsWithCopy(min_root_tree, heap->root, &tmp, heap->cmp);
        BinomialHeapFreeFromTree(heap->root);
        BinomialHeapFreeFromTree(min_root_tree);
        heap->root = tmp;        
    }
    else {
        BinomialHeapFreeTree(min_root_tree);
    }
    return SUCCESS;
}

Request* BinomialHeapGetMax(BinomialHeapPQ* heap) {
    if (!heap || !heap->root) return INC_ARGS;
    return heap->root->head->data;
}



kErrors BinomialHeapMergeWithDestruction(BinomialHeapPQ* heap1, BinomialHeapPQ* heap2, BinomialHeapPQ* merged) {
    if (!heap1 || !heap2) return INC_ARGS;
    kErrors status = SUCCESS;
    if (status != SUCCESS) {
        return status;
    }
    status = BinomialHeapMergeWithoutDestruction(heap1, heap2, merged);
    if (status != SUCCESS) {
        return status;
    }
    merged->size = heap1->size + heap2->size;
    BinomialHeapFree(heap1);
    BinomialHeapFree(heap2);
    return SUCCESS;
}


kErrors BinomialHeapMergeWithoutDestruction(BinomialHeapPQ* pq1, BinomialHeapPQ* pq2, BinomialHeapPQ* merged) {
    kErrors status = BinomialHeapMergeByRootsWithCopy(pq1->root, pq2->root, &(merged->root), pq1->cmp);
    if (status != SUCCESS) {
        return status;
    }
    merged->size = pq1->size + pq2->size;
    return SUCCESS;
}

void BinomialHeapFreeRecursively(BinomialHeapNode* node) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < node->children_size; i++) {
        BinomialHeapFreeRecursively(node->children[i]);
    }
    free(node->children);
    free(node);
}

void BinomialHeapFreeTree(BinomialTree* tree) {
    if (tree == NULL) {
        return;
    }
    BinomialTree* tmp;
    BinomialHeapFreeRecursively(tree->head);
    free(tree);
}
void BinomialHeapFree(BinomialHeapPQ* heap) {
    BinomialHeapFreeFromTree(heap->root);
    free(heap);
}
void BinomialHeapFreeFromTree(BinomialTree* tree) {
    BinomialTree* cur = tree;
    BinomialTree* tmp;

    while (cur != NULL) {
        tmp = cur->sibling;
        BinomialHeapFreeTree(cur);
        cur = tmp;
    }
}

kErrors BinomialHeapCopyOneTree(BinomialTree** dest, BinomialTree* source) {

    if (*dest != NULL) {
        BinomialHeapFreeTree(*dest);
    }

    BinomialHeapCreateTree(dest, NULL);
    if (*dest == NULL) {
        return MEM_ALLOC_ERR;
    }
    
    BinomialHeapCopy(&(*dest)->head, source->head);
    (*dest)->degree = source->degree;
    return SUCCESS;
}

kErrors BinomialHeapCopyFromTree(BinomialTree** dest, BinomialTree* source) {
    BinomialTree* cur = source;
    while (cur != NULL) {
        if (*dest == NULL) {
            BinomialHeapCreateTree(dest, NULL);
            if (*dest == NULL) {
                return MEM_ALLOC_ERR;
            }
        }
        BinomialHeapCopy(&(*dest)->head, cur->head);
        (*dest)->degree = cur->degree;
        dest = &(*dest)->sibling;
        cur = cur->sibling;
    }
}

kErrors BinomialHeapCopy(BinomialHeapNode** dest, BinomialHeapNode* source) {
    kErrors status = SUCCESS;
    if (source == NULL) {
        return SUCCESS;
    }

    if (*dest != NULL) {
        BinomialHeapFreeRecursively(*dest);
    }

    
    *dest = BinomialHeapCreateNode(source->key, source->data);
    if (*dest == NULL) {
        return MEM_ALLOC_ERR;
    }
    (*dest)->children_capacity = source->children_capacity;
    (*dest)->children_size = source->children_size;
    for (int i = 0; i < source->children_size; i++) {
        BinomialHeapCopy(&(*dest)->children[i], source->children[i]);
    }
    return status;
}


void PrintChildren(BinomialHeapNode* node) {
    printf("%d, c: [", node->key);
    for (int i = 0; i < node->children_size; i++) {        
        PrintChildren(node->children[i]);
    }
    printf("]");
}

void BinomialHeapPrint(BinomialHeapPQ* pq) {
    BinomialTree* cur = pq->root;
    while (cur != NULL) {
        PrintChildren(cur->head);
        printf("\n");
        cur = cur->sibling;
    }
}

kErrors BinomialHeapMeld(BinomialHeapPQ* p_in, BinomialHeapPQ* p_out) {
    kErrors status = SUCCESS;
    BinomialHeapNode* cur = NULL;
    Request* cur_req = NULL;
    while (p_in->size > 0) {
        status = BinomialHeapDeleteMax(p_in, &cur_req);
        if (status != SUCCESS) {
            return status;
        }
        status = BinomialHeapInsert(p_out, cur_req->priority, cur_req);
        if (status != SUCCESS) {
            return status;
        }
    }
    return SUCCESS;
}