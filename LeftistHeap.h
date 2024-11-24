#pragma once
#include "OpsAndDeps.h"
#define TYPE Request*

// ��������� ��� �������� ������������� ����
typedef struct LeftistHeapNode {
  int key;                        // ���������
  TYPE data;                      // ������
  struct LeftistHeapNode* left;   // ����� �������
  struct LeftistHeapNode* right;  // ������ �������
  int npl;  // ������������ ����� ���� (null path length)
} LeftistHeapNode;

// ��������� ��� ������������ ������� (������������� ����)
typedef struct {
  LeftistHeapNode* root;  // ������ ����
  bool (*cmp)(int, int);
  int size;
} LeftistHeapPQ;

LeftistHeapNode* LeftistHeapCreateNode(int key, TYPE data);

LeftistHeapNode* LeftistHeapMerge(LeftistHeapNode* h1, LeftistHeapNode* h2,
                                  bool (*cmp)(int, int));

kErrors LeftistHeapCreatePriorityQueue(LeftistHeapPQ* pq,
                                       bool (*cmp)(int, int));

kErrors LeftistHeapInsert(LeftistHeapPQ* pq, int key, TYPE data);

kErrors LeftistHeapDeleteMax(LeftistHeapPQ* pq,
                             Request** max);

kErrors LeftistHeapGetMax(LeftistHeapPQ* pq, LeftistHeapNode** max);

kErrors LeftistHeapMergeWithDestruction(LeftistHeapPQ* pq1,
                                        LeftistHeapPQ* pq2);

void LeftistHeapFreePriorityQueue(LeftistHeapPQ* pq);

void LeftistHeapPrint(LeftistHeapPQ* p);
kErrors LeftistHeapMeld(LeftistHeapPQ* p_in, LeftistHeapPQ* p_out);
void LeftistHeapPrintNodes(LeftistHeapNode* root, int tab_count);
bool CmpMax(int a, int b);
void LeftistHeapRecursiveFree(LeftistHeapNode* node);
kErrors LeftistHeapCopyPqFromRoot(LeftistHeapPQ* dest, LeftistHeapNode* source);