#define _CRT_SECURE_NO_WARNINGS
#include "myerrors.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "DynamicArray.h"

kErrors DArrayCreate(DArray* arr, int(*_cmp)(void*, void*), int (*_cmpById)(char*, char*))
{
    arr->capacity = 10;
    arr->cmp = _cmp;
    arr->cmpById = _cmpById;
    arr->buffer = (DANode**)calloc(10, sizeof(DANode*));
    if (arr->buffer == NULL) {
        return MEM_ALLOC_ERR;
    }
    arr->size = 0;
    return SUCCESS;
}

DANode* DArrayCreateNode(Department* data, const char* id)
{
    DANode* n = (DANode*)malloc(sizeof(DANode));
    if (n == NULL) {
        return NULL;
    }
    n->data = data;
    n->id = (char*)malloc(256);
    if (n->id == NULL) {
        free(n);
        return NULL;
    }
    strcpy(n->id, id);
    return n;
}

kErrors DArrayInsert(DArray* arr, Department* data, const char* id)
{
    DANode* new = DArrayCreateNode(data, id);
    if (new == NULL) {
        return MEM_ALLOC_ERR;
    }

    if (arr->size == arr->capacity) {
        DANode** temp = arr->buffer;
        arr->buffer = (DANode**)realloc(arr->buffer, sizeof(DANode*) * arr->capacity * 2);
        if (arr->buffer == NULL) {
            arr->buffer = temp;
            return MEM_ALLOC_ERR;
        }
        arr->capacity *= 2;
    }
    arr->buffer[arr->size++] = new;
    qsort(arr->buffer, arr->size, sizeof(DANode*), arr->cmp);
    return SUCCESS;
}

void DAFreeNode(DANode* node)
{
    free(node->id);
    free(node);
}

kErrors DArrayDelete(DArray* arr, char* id)
{
    int ind;
    kErrors status = DAFindIndex(arr, id, &ind);
    if (status != SUCCESS) {
        return status;
    }
    DAFreeNode(arr->buffer[ind]);
    for (int i = ind; i < arr->size - 1; i++) {
        arr->buffer[i] = arr->buffer[i + 1];
    }
    arr->size--;
    return SUCCESS;
}

void DAPrintNode(DANode* node)
{
    printf("Id: %s\n", node->id);
    SkewHeapPrint(node->data->req_queue);
}


void DArrayPrint(DArray* arr)
{
    for (int i = 0; i < arr->size; i++) {
        DAPrintNode(arr->buffer[i]);
    }
    printf("\n");
}

kErrors DAFindIndex(DArray* arr, char* id, int* out){
    int l = 0, r = arr->size;
    int c = (l + r) / 2;
    while (r - l > 0)
    {
        if (arr->cmpById(id, arr->buffer[c]->id) < 0)
            r = c;
        else if (arr->cmpById(id, arr->buffer[c]->id) > 0)
            l = c;
        else
            break;
        c = (l + r) / 2;
    }
    if (strcmp(arr->buffer[c]->id, id) != 0) {
        return NO_SUCH_EL;
    }
    *out = c;
    return SUCCESS;
}

Department* DArraySearch(DArray* arr, char* id)
{
    int ind;
    if (DAFindIndex(arr, id, &ind) != SUCCESS) {
        return NULL;
    }
    else {
        return arr->buffer[ind]->data;
    }
}

void DArrayFree(DArray* arr)
{
    for (int i = 0; i < arr->size; i++) {
        DAFreeNode(arr->buffer[i]);
    }
    free(arr->buffer);
}
