#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

typedef struct {
    int pid;
    int ms;
} task_descr_t;

typedef struct task_node_t {
    struct task_node_t* next;
    task_descr_t current;
} task_node_t;

typedef struct {
    task_node_t* head;
    task_node_t* tail;
    int size;
} task_queue_t;

task_queue_t* task_queue_init();

int task_queue_empty(task_queue_t* queue);

void task_push(task_queue_t* queue, task_descr_t item, pthread_mutex_t* mutex);

task_descr_t task_pop(task_queue_t* queue, pthread_mutex_t* mutex);

void task_queue_print(task_queue_t* queue);

#endif
