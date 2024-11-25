#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct Request Request;
typedef struct queue_req queue_req;
#include "OpsAndDeps.h"
#define TYPE Request*


typedef struct queue_req {
    TYPE *buffer;
    int size;
    int first;
    int buffer_size;
} queue_req;
bool queue_create(queue_req *q, int init_buff_size);
void queue_destroy(queue_req *q);
bool queue_is_empty(queue_req *q);
bool queue_push(queue_req *q, TYPE d);
void queue_pop(queue_req *q);
TYPE queue_front(queue_req* q);
bool queue_grow(queue_req* q);
void queue_shrink(queue_req* q);

