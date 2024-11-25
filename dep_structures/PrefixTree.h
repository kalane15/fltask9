#pragma once

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "OpsAndDeps.h"

#define ALPHABET_SIZE 63

typedef struct trieNode {
	struct trieNode *children[ALPHABET_SIZE];
	Department* data;
	bool is_end_of_word;
} trieNode;

typedef struct Trie {
	trieNode* root;
} Trie;

int charToInt(char x);
kErrors TrieCreate(Trie* trie);
trieNode *TrieCreateNode(Department* data);

kErrors TrieInsert(Trie* trie, const char* word, Department* data);

Department* TrieSearch(Trie* trie, const char* word);

void TrieFree(Trie* trie);

void TriePrint(Trie* trie, char* prefix, int depth);
