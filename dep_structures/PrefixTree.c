#define _CRT_SECURE_NO_WARNINGS
#include "PrefixTree.h"
#include "myerrors.h"

#define ALPHABET_SIZE 63

int charToInt(char x) {
	if (isdigit(x)) {
		return x - '0';
	} else if (islower(x)) {
		return x - 'a' + 10;
	} else if (isupper(x)) {
		return x - 'A' + 36;
	} else if (x == '_') {
		return 62;
	}
	return -1;
}

kErrors TrieCreate(Trie* trie) {
	trie->root = NULL;
	return SUCCESS;
}

trieNode *TrieCreateNode(Department* data) {
	trieNode *node = (trieNode *)malloc(sizeof(trieNode));
	if (node == NULL) {
		return MEM_ALLOC_ERR;
	}
	node->data = data;
	if (node == NULL) {
		return NULL;
	}
	node->is_end_of_word = false;
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		node->children[i] = NULL;
	}
	return node;
}

kErrors TrieInsert(Trie *trie, const char *word, Department* data) {
	if (trie->root == NULL) {
		trie->root = TrieCreateNode(NULL);
		if (trie->root == NULL) {
			return MEM_ALLOC_ERR;
		}
	}
	trieNode* current = trie->root;
	while (*word) {
		int index = charToInt(*word);
		if (index < 0 || index >= ALPHABET_SIZE) {
			return INC_INP_DATA;
		}
		if (current->children[index] == NULL) {
			current->children[index] = TrieCreateNode(NULL);
			if (current->children[index] == NULL) {
				return MEM_ALLOC_ERR;
			}
		}
		current = current->children[index];
		word++;
	}
	current->data = data;
	current->is_end_of_word = true;
	return SUCCESS;
}

Department* TrieSearch(Trie *trie, const char *word) {
	trieNode* current = trie->root;
	if (current == NULL) {
		return NULL;
	}	
	while (*word) {
		int index = charToInt(*word);
		if (index < 0 || index >= ALPHABET_SIZE) {
			return NULL;
		}
		if (current->children[index] == NULL) {
			return NULL;
		}
		current = current->children[index];
		word++;
	}
	return current->data;
}

void TrieFreeRecursive(trieNode* root, MainModel* model) {
	if (root->is_end_of_word) {
		DepFree(root->data, model->req_store_type);
	}
	for (int i = 0; i < ALPHABET_SIZE; ++i) {
		if (root->children[i]) {
			TrieFreeRecursive(root->children[i], model);
		}
	}
	free(root);
}

void TrieFree(Trie *trie, MainModel* model) {
	TrieFreeRecursive(trie->root, model);
}

void TriePrint(Trie *trie, char *prefix, int depth) {
	trieNode* root = trie->root;
	if (root == NULL) {
        return;
    }

    // Печатаем текущий узел
    if (root->is_end_of_word) {
        printf("%*s%s: %u\n", depth * 2, "", prefix, root->is_end_of_word);
    }

    // Рекурсивно печатаем потомков
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (root->children[i]) {
            char new_prefix[100]; // Максимальная длина префикса
            // Исправленный код для символа '_':
            sprintf(new_prefix, "%s%c", prefix, 
                    i < 10 ? i + '0' : 
                    i < 36 ? i - 10 + 'A' :
                    i == 62 ? '_' : i - 36 + 'a');
            TriePrint(root->children[i], new_prefix, depth + 1);
        }
    }
}