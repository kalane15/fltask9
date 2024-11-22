#define _CRT_SECURE_NO_WARNINGS
#include "hashTable.h"

// Функция вычисления хэша
unsigned int Hash(int table_size, char *str) {
    unsigned int hash = 0;
    unsigned int power = 1;
    while (*str) {
        hash = (hash + (*str) * power) % table_size;
        power = (power * PRIME) % table_size;
        str++;
    }
    return hash;
}

// Создание хэш таблицы
kErrors HashTableCreate(HashTable* table, int size) {
    table->table = malloc(size * sizeof(HashTableNode *));
    if (table->table == NULL) {
        return MEM_ALLOC_ERR;
    }
    for (int i = 0; i < size; i++) {
        table->table[i] = NULL;
    }
    table->size = size;
    return SUCCESS;
}


// Вставка в хэш таблицу
kErrors HashTableInsert(HashTable *table, char *key, Department* value) {
    unsigned int index = Hash(table->size, key);
    HashTableNode *new_node = malloc(sizeof(HashTableNode));
    if (new_node == NULL) {
        return MEM_ALLOC_ERR;
    }
    new_node->key = malloc(256);
    if (new_node->key == NULL) {
        return MEM_ALLOC_ERR;
    }
    strcpy(new_node->key, key);
    new_node->value = value;
    new_node->next = table->table[index];
    table->table[index] = new_node;
    return SUCCESS;
}


// Поиск по хэш таблице
Department * HashTableSearch(HashTable *table, char *key) {
    if (key == NULL){
        return NULL;
    }
    unsigned int index = Hash(table->size, key);
    HashTableNode *current = table->table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

void HashTableFree(HashTable *table, MainModel* model) {
    for (int i = 0; i < table->size; i++) {
        HashTableNode *current = table->table[i];
        while (current != NULL) {
            HashTableNode *next = current->next;
            free(current->key);
            DepFree(current->value, model->req_store_type);
            free(current);
            current = next;
        }
    }
    free(table->table);
    free(table);
}