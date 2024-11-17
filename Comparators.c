#include "string.h"
#include "DynamicArray.h"

int cmpId(char* a, char* b) {
	return strcmp(a, b);
}

int cmpDA(void* a, void* b) {
	DANode** _a = (DANode**)a;
	DANode** _b = (DANode**)b;
	return cmpId((*_a)->id, (*_b)->id);
}

bool cmpMax(int a, int b) {
	return a < b;
}