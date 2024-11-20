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
kErrors BinomialHeapCreatePriorityQueue(BinomialHeapPriorityQueue* out, bool (*inp_cmp)(int*, int*)) {
    out->cmp = inp_cmp;
    out->root = NULL;
    out->size = 0;
    return SUCCESS;
}
//
//// Добавление элемента в косую кучу
kErrors BinomialHeapInsert(BinomialHeapPriorityQueue* heap, int key, Request* data) {
    if (!heap) return INC_ARGS; // Пустая очередь

    BinomialHeapNode* newNode = BinomialHeapCreateNode(key, data);
    if (!newNode) return MEM_ALLOC_ERR; // Ошибка выделения памяти

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
//
//// Удаление максимального элемента (по ключу)
//kErrors SkewHeapDeleteMax(SkewHeapPriorityQueue* heap, Request** data) {
//    if (!heap || !heap->root) return INC_ARGS; // Пустая очередь
//    kErrors status = SUCCESS;
//    SkewHeapNode* root = heap->root;
//    *data = root->data;
//    SkewHeapNode* temp = NULL;
//    // Слияние левого и правого поддеревьев
//    status = SkewHeapMergeByRootsWithCopy(heap->root->left, heap->root->right, &temp, heap->cmp);
//    if (status != SUCCESS) {
//        return status;
//    }
//    heap->root = temp;
//    SkewHeapFreeRecursively(root);
//    heap->size--;
//    return SUCCESS;
//}
//
kErrors BinomialHeapDeleteMax(BinomialHeapPriorityQueue* heap, Request** data) {
    if (!heap || !heap->root) return INC_ARGS; // Пустая очередь
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
        DepFree(min_root_tree->head->data, BINOMIAL_HEAP);
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
        DepFree(min_root_tree->head->data, BINOMIAL_HEAP);
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
//
//// Поиск максимального элемента (по ключу)
//kErrors SkewHeapGetMax(SkewHeapPriorityQueue* heap, SkewHeapNode* data) {
//    if (!heap || !heap->root) return INC_ARGS; // Пустая очередь
//
//    *data = *(heap->root);
//    return SUCCESS;
//}
//
//
////Слияние двух очередей с разрушением исходных
//kErrors SkewHeapMergeWithDestruction(SkewHeapPriorityQueue* heap1, SkewHeapPriorityQueue* heap2, SkewHeapPriorityQueue* merged) {
//    if (!heap1 || !heap2) return INC_ARGS;
//    kErrors status = SUCCESS;
//    if (status != SUCCESS) {
//        return status;
//    }
//    status = SkewHeapMergeWithoutDestruction(heap1, heap2, merged);
//    if (status != SUCCESS) {
//        return status;
//    }
//    merged->size = heap1->size + heap2->size;
//    SkewHeapFree(heap1);
//    SkewHeapFree(heap2);
//    return SUCCESS;
//}
//


void BinomialHeapFreeRecursively(BinomialHeapNode* node) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < node->children_size; i++) {
        BinomialHeapFreeRecursively(node->children[i]);
    }
    DepFree(node->data, BINOMIAL_HEAP);
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
void BinomialHeapFree(BinomialHeapPriorityQueue* heap) {
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

void BinomialHeapPrint(BinomialHeapPriorityQueue* pq) {
    BinomialTree* cur = pq->root;
    while (cur != NULL) {
        PrintChildren(cur->head);
        printf("\n");
        cur = cur->sibling;
    }
}
//
//kErrors SkewHeapCopyPqFromRoot(SkewHeapPriorityQueue* dest, SkewHeapNode* source) {
//    if (source == NULL) {
//        return SUCCESS;
//    }
//    return SkewHeapCopy(&(dest->root), source);
//}
//
//void SkewHeapPrintNode(SkewHeapNode* node) {
//    char* buffer = (char*)malloc(1024);
//    strftime(buffer, 1024, "%Y-%m-%d %H:%M:%S", &(node->data->req_time_get));
//    printf("Time get: %s, pr: %d, dep_id: %s, text: %s \n",
//        buffer, node->data->priority, node->data->dep_id, node->data->text);
//}
//
//void SkewHeapPrintNodes(SkewHeapNode* root, int tab_count) {
//    if (root != NULL)
//    {
//        SkewHeapPrintNodes(root->right, tab_count + 1);
//        for (int i = 0; i < tab_count; ++i) { printf("\t"); }
//        SkewHeapPrintNode(root);
//        SkewHeapPrintNodes(root->left, tab_count + 1);
//    }
//}
//
//void SkewHeapPrint(SkewHeapPriorityQueue* p)
//{
//    SkewHeapPrintNodes(p->root, 0);
//}
//
kErrors BinomialHeapMeld(BinomialHeapPriorityQueue* p_in, BinomialHeapPriorityQueue* p_out) {
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