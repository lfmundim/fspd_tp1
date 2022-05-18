#include "taskQueue.h"

task_queue_t* task_queue_init(){
    task_queue_t* queue = malloc(sizeof(task_queue_t));
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

int task_queue_empty(task_queue_t* queue){
    return queue->size == 0;
}

void task_push(task_queue_t* queue, task_descr_t item, pthread_mutex_t* mutex){
    pthread_mutex_lock(mutex);
    task_node_t* node = malloc(sizeof(task_node_t));
    node->current = item;
    node->next = NULL;
    if(queue->head == NULL){
        queue->head = node;
    } else {
        queue->tail->next = node;
    }
    queue->tail = node;
    queue->size++;
    pthread_mutex_unlock(mutex);
    task_queue_print(queue);
}

task_descr_t task_pop(task_queue_t* queue, pthread_mutex_t* mutex){
    pthread_mutex_lock(mutex);

    if(queue->size == 0) {
        pthread_mutex_unlock(mutex);
        task_descr_t EOW = {
            .pid = -1,
            .ms = -1
        };
        return EOW;
    }

    task_descr_t value = queue->head->current;
    task_node_t* temp = queue->head;
    queue->head = queue->head->next;
    queue->size--;
    free(temp);

    pthread_mutex_unlock(mutex);

    return value;
}

void task_queue_print(task_queue_t* queue){
    task_node_t* task;
    for(task = queue->head; task != NULL; task = task->next){
        printf("%d ", task->current.pid);
    }
}
