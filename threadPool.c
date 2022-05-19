#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "threadPool.h"
#include "taskQueue.h"

thread_pool_t* thread_pool_init(int min_size, int max_size){
    thread_pool_t* thread_pool = malloc(sizeof(thread_pool));
    thread_pool->min_size = min_size;
    thread_pool->max_size = max_size;
    thread_pool->busy_threads = thread_queue_init();
    thread_pool->not_busy_threads = thread_queue_init();

    return thread_pool;
}

void thread_run(pthread_t* thread, task_descr_t task){

}
