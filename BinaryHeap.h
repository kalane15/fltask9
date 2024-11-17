#pragma once
#include "OpsAndDeps.h"
#define TYPE Request*
#include "stdbool.h"

// ��������� ��� �������� �������� ����
typedef struct {
    int key;    // ���������
    TYPE data;  // ������, ������ ���� TYPE
} BinaryHeapNode;

// ��������� ��� ������������ ������� (�������� ����)
typedef struct {
    BinaryHeapNode* heap;  // ������ ��������� ����
    bool (*cmp)(int, int);
    int size;                 // ������� ������ ����
    int capacity;             // ����������� ����    
} BinaryHeapPriorityQueue;

void BinaryHeapSwap(BinaryHeapNode* a, BinaryHeapNode* b);

void BinaryHeapHeapify(BinaryHeapPriorityQueue* pq, int index);

kErrors BinaryHeapCreatePriorityQueue(int capacity, BinaryHeapPriorityQueue* pq, bool (*)(int, int));

kErrors BinaryHeapResize(BinaryHeapPriorityQueue* pq);

kErrors BinaryHeapInsert(BinaryHeapPriorityQueue* pq, int key, TYPE data);

kErrors BinaryHeapDeleteMax(BinaryHeapPriorityQueue* pq, Request** max);

kErrors BinaryHeapGetMax(BinaryHeapPriorityQueue* pq, BinaryHeapNode** max);

kErrors BinaryHeapMergeWithoutDestruction(BinaryHeapPriorityQueue* pq1, BinaryHeapPriorityQueue* pq2, BinaryHeapPriorityQueue** merged);

kErrors BinaryHeapMergeWithDestruction(BinaryHeapPriorityQueue* pq1, BinaryHeapPriorityQueue* pq2);

void BinaryHeapFreePriorityQueue(BinaryHeapPriorityQueue* pq);

kErrors BinaryHeapMeld(BinaryHeapPriorityQueue* p_in, BinaryHeapPriorityQueue* p_out);
