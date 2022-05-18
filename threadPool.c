#include <stdio.h>
#include <stdlib.h>
#include "threadPool.h"

thread_pool_t* thread_pool_init(int min_size, int max_size){
    thread_pool_t* thread_pool = malloc(sizeof(thread_pool));
    thread_pool->min_size = min_size;
    thread_pool->max_size = max_size;
    thread_pool->thread_queue = thread_queue_init();

    return thread_pool;
}
