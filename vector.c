#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "vector.h"

bool vector_create(vector* v, int init_buff_size) {
	v->size = 0;
	v->buffer_size = init_buff_size;
	v->buffer = malloc(sizeof(TYPE) * init_buff_size);
	return v->buffer != NULL;
}

bool vector_is_empty(vector* v) {
	return v->size == 0;
}

bool vector_push(vector* v, TYPE d) {	
	bool res;
	if (v->size == v->buffer_size) {
		res = vector_grow(v);
		if (res) {
			v->buffer[v->size] = d;
			v->size++;
			return true;
		}
		else {
			return false;
		}
	}
	v->buffer[v->size] = d;
	v->size++;
	return true;
}

void v_set(vector* v, int pos, TYPE val) {
	v->buffer[pos] = val;
}

TYPE v_get(vector* v, int pos) {
	return v->buffer[pos];
}

bool vector_grow(vector* v) {
	TYPE* new_buf = (TYPE*)realloc(v->buffer, v->buffer_size * 2 * sizeof(TYPE));
	if (new_buf == NULL) {
		return false;
	}
	else {
		v->buffer_size *= 2;
		v->buffer = new_buf;
		return true;
	}
}

void vector_destroy(vector* v) {
	v->size = 0;
	free(v->buffer);
}

void vector_delete(vector* v, int index) {
	RequestFree(v->buffer[index]);
	for (int i = index; i < v->size - 1; i++) {
		v->buffer[i] = v->buffer[i+1];
	}
	v->size--;
}