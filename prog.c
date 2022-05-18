#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "taskQueue.h"
#include "threadQueue.h"
#include "threadPool.h"

pthread_mutex_t mutex;
pthread_cond_t cond;

#define true 1
#define false 0

void processa(task_descr_t* tp)
{
    struct timespec zzz;

    zzz.tv_sec  = tp->ms/1000;
    zzz.tv_nsec = (tp->ms%1000) * 1000000L;

    printf("IP #%d\n", tp->pid);
    nanosleep(&zzz,NULL);
    printf("FP #%d\n", tp->pid);
}

int main(int argc, char *argv[])
{
    task_queue_t* task_queue = task_queue_init();
    int min_threads = argv[0];
    int max_threads = argv[1];
    thread_pool_t* thread_pool = thread_pool_init(min_threads, max_threads);

    // read task description inputs
    while(true){
        int pid, ms;
        scanf("%d %d", &pid, &ms);
        if(pid == EOF){
            break;
        } else {
            task_descr_t task = {
                .pid = pid,
                .ms = ms
            };
            task_push(task_queue, task, &mutex);
        }
    }

    while(!task_queue_empty(task_queue)){
        task_descr_t task = task_pop(task_queue, &mutex);
        processa(&task);
    }
}
