#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "threadQueue.h"

void thread_push(thread_queue_t* queue, pthread_t* thread, pthread_mutex_t* mutex){
    pthread_mutex_lock(mutex);
    thread_node_t* node = malloc(sizeof(thread_node_t));
    node->current = thread;
    node->next = NULL;

    if(queue->head == NULL && queue->tail == NULL){
        // no nodes on queue, first node is head and tail
        queue->head = node;
        queue->tail = node;
        queue->size = 1;
    } else {
        // nodes on queue, current node is tail
        queue->tail->next = node;
        queue->tail = node;
        queue->size++;
    }
    pthread_mutex_unlock(mutex);
}

pthread_t* thread_pop(thread_queue_t* queue, pthread_mutex_t* mutex){
    pthread_mutex_lock(mutex);

    if(queue->size == 0){
        // nothing to pop, returning
        pthread_mutex_unlock(mutex);
        return NULL;
    }

    pthread_t* value = queue->head->current;
    // get pointer to what is being popped to free
    thread_node_t* temp = queue->head;
    queue->head = queue->head->next;
    queue->size--;
    free(temp);

    pthread_mutex_unlock(mutex);
    return value;
}

thread_queue_t* thread_queue_init(){
    thread_queue_t* queue = malloc(sizeof(thread_queue_t));
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}
