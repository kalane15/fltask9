#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "queue_req.h"


void queue_create(queue_req* q, int init_buff_size) {
	q->size = 0;
	q->first = 0;
	q->buffer_size = init_buff_size;
	q->buffer = malloc(sizeof(TYPE) * init_buff_size);
}

void queue_destroy(queue_req* q) {
	q->size = 0;
	q->first = 0;
	free(q->buffer);
	free(q);
}

bool queue_is_empty(queue_req* q) {
	return q->size == 0;
}

bool queue_push(queue_req* q, TYPE d) {
	bool free_place = true;

	if (q->size == q->buffer_size) {
		free_place = queue_grow(q);
	}

	q->buffer[(q->first + q->size) % q->buffer_size] = d;
	q->size++;
	return true;
}

void queue_pop(queue_req* q) {
	if (q->size == 0) {
		return;
	}
	q->size--;
	if (q->size == q->buffer_size * 4 / 9) {
		//queue_shrink(q);
	}
	q->first = (q->first + 1) % q->buffer_size;
}

bool queue_grow(queue_req* q) {
	TYPE* new_buf = (TYPE*)realloc(q->buffer, q->buffer_size * 2 * sizeof(TYPE));
	if (new_buf == NULL) {
		return false;
	}
	else {
		q->buffer_size *= 2;
		q->buffer = new_buf;
		return true;
	}
}

void queue_shrink(queue_req* q) {
	if (q->size == 0) {
		return;
	}
	queue_req q_temp;
	queue_create(&q_temp, q->size);

	while (q->size > 0) {
		queue_push(&q_temp, queue_front(q));
		queue_pop(q);
	}

	q->first = 0;
	while (q_temp.size > 0) {
		queue_push(q, queue_front(&q_temp));
		queue_pop(&q_temp);
	}

	q->buffer = (TYPE*)realloc(q->buffer, q->buffer_size * sizeof(TYPE) * 2 / 3);
	q->buffer_size = q->buffer_size * 2 / 3;
}

TYPE queue_front(queue_req* q) {
	return (q->buffer[q->first]);
}

void print_q(queue_req* q) {
	queue_req temp;
	queue_create(&temp, q->buffer_size);

	while (!queue_is_empty(q)) {
		queue_push(&temp, queue_front(q));
		printf("%d ", queue_front(q));
		queue_pop(q);
	}
	printf("\n");
	while (!queue_is_empty(&temp)) {
		queue_push(q, queue_front(&temp));
		queue_pop(&temp);
	}
}