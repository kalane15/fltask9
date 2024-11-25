#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "OpsAndDeps.h"


#define PRIME 97

typedef struct Node {
	char *key;
	Department* value;
	struct Node *next;
} HashTableNode;

typedef struct HashTable {
	HashTableNode **table;
	int size;
} HashTable;

unsigned int Hash(int size, char* str);

kErrors HashTableCreate(HashTable* t, int size);

kErrors HashTableInsert(HashTable* table, char* key, Department* value);

Department* HashTableSearch(HashTable* table, char* key);

void HashTableFree(HashTable* table);
