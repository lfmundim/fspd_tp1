#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex;

typedef struct {
    int pid;
    int ms;
} task_descr_t;

typedef struct {
    task_descr_t* next;
    task_descr_t* current;
} node_t;

typedef struct {
    node_t* head;
    node_t* tail;
    int size;
} queue_t;

queue_t* queue_init(){
    queue_t* queue = malloc(sizeof(queue_t));
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

void push(queue_t* queue, task_descr_t* item){
    pthread_mutex_lock(&mutex);
    node_t* node = malloc(sizeof(node_t));
    node->current = item;
    node->next = NULL;
    if(queue->head == NULL && queue->tail == NULL){
        queue->head = node;
        queue->tail = node;
        queue->size = 1;
    } else {
        queue->tail->next = node;
        queue->tail = node;
        queue->size++;
    }
    pthread_mutex_unlock(&mutex);
}

void* pop(queue_t* queue){
    pthread_mutex_lock(&mutex);

    if(queue->size == 0) {
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    task_descr_t* value = queue->head->current;
    node_t* temp = queue->head;
    queue->head = queue->head->next;
    queue->size--;
    free(temp);

    pthread_mutex_unlock(&mutex);

    return value;
}

void processa(task_descr_t* tp)
{
    struct timespec zzz;

    zzz.tv_sec = 0;
    zzz.tv_nsec = tp->ms * 1000000L; // 0 <= ms <= 999

    printf("IP #%d\n", tp->pid);
    nanosleep(&zzz,NULL);
    printf("FP #%d\n", tp->pid);
}

int main(int argc, char *argv[])
{
    queue_t* queue = queue_init();
    int min_threads = argv[0];
    int max_threads = argv[1];
    while(1){

    }
}