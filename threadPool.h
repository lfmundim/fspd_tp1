#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "threadQueue.h"

typedef struct {
    int min_size;
    int max_size;
    thread_queue_t* thread_queue;
} thread_pool_t;

thread_pool_t* thread_pool_init(int min_size, int max_size);

#endif
