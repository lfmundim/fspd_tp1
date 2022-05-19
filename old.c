#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "taskQueue.h"
#include "threadQueue.h"
#include "threadPool.h"

pthread_mutex_t mutex;
pthread_cond_t max_threads_cond;
pthread_cond_t no_threads_cond;
pthread_attr_t attr;
int running_threads;
int min_threads;
int max_threads;

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

void execute(task_descr_t* tp){
    while(true){
        while(running_threads == max_threads){
            pthread_cond_wait(&max_threads_cond, &mutex);
        }
        pthread_mutex_lock(&mutex);
        running_threads++;
        pthread_mutex_unlock(&mutex);

        processa(tp);

        pthread_mutex_lock(&mutex);
        running_threads--;
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char *argv[])
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&max_threads_cond, NULL);
    pthread_cond_init(&no_threads_cond, NULL);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    task_queue_t* task_queue = task_queue_init();
    min_threads = argv[0];
    max_threads = argv[1];
    running_threads = 0;
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

    thread_pool->not_busy_threads = (pthread_t*)calloc(max_threads, sizeof(pthread_t));

    while(!task_queue_empty(task_queue)){
        task_descr_t task = task_pop(task_queue, &mutex);
        while(thread_queue_empty(&thread_pool->not_busy_threads)){
            pthread_cond_wait(&no_threads_cond, &mutex);
        }
        pthread_t thread = thread_pop(thread_pool->not_busy_threads, &mutex);
        pthread_create(&thread, &attr, execute, &task);
        thread_push(thread_pool->not_busy_threads, &thread, &mutex);
        pthread_cond_signal(&no_threads_cond);
        pthread_cond_signal(&max_threads_cond);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&max_threads_cond);
    pthread_cond_destroy(&no_threads_cond);
}
