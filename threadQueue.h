#ifndef THREADQUEUE_H
#define THREADQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct thread_node_t {
    struct thread_node_t* next;
    pthread_t* current;
} thread_node_t;

typedef struct {
    thread_node_t* head;
    thread_node_t* tail;
    int size;
} thread_queue_t;

int thread_queue_empty(thread_queue_t* queue);

void thread_push(thread_queue_t* queue, pthread_t* thread, pthread_mutex_t* mutex);

pthread_t* thread_pop(thread_queue_t* queue, pthread_mutex_t* mutex);

thread_queue_t* thread_queue_init();

#endif
