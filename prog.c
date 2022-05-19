#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define true 1
#define false 0

typedef struct {
    int pid;
    int ms;
} task_descr_t;

task_descr_t task_queue[256];
int min_threads;
int max_threads;
int task_count;
int thread_count;
pthread_mutex_t mutex_queue;
pthread_attr_t attr;
pthread_cond_t cond_queue;

void processa(task_descr_t* tp)
{
    struct timespec zzz;

    zzz.tv_sec  = tp->ms/1000;
    zzz.tv_nsec = (tp->ms%1000) * 1000000L;

    printf("IP #%d\n", tp->pid);
    nanosleep(&zzz,NULL);
    printf("FP #%d\n", tp->pid);
}

void* start_thread(void* args){
    while(true){
        task_descr_t task;
        pthread_mutex_lock(&mutex_queue);
        if(task_count == 0) {
            thread_count--;
            pthread_mutex_unlock(&mutex_queue);
            break;
        }
        task = task_queue[0];
        for(int i = 0; i<task_count-1; i++){
            task_queue[i] = task_queue[i+1];
        }
        task_count--;
        pthread_mutex_unlock(&mutex_queue);

        processa(&task);
        pthread_mutex_lock(&mutex_queue);
        if(task_count < thread_count && thread_count > min_threads) {
            thread_count--;
            pthread_mutex_unlock(&mutex_queue);
            break;
        }
        pthread_mutex_unlock(&mutex_queue);
    }
}

int main(int argc, char *argv[])
{
    min_threads = atoi(argv[1]);
    max_threads = atoi(argv[2]);
    // printf("MIN THREADS: %d | MAX THREADS %d\n", min_threads, max_threads);
    int i = 0;
    task_count = 0;
    pthread_mutex_init(&mutex_queue, NULL);
    pthread_attr_init(&attr);
    pthread_cond_init(&cond_queue, NULL);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

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
            task_queue[i++] = task;
            task_count++;
        }
    }

    pthread_t threads[max_threads];
    int j;
    for(j = 0; j<max_threads; j++){
        thread_count++;
        pthread_create(&threads[j], &attr, &start_thread, NULL);
    }

    pthread_mutex_destroy(&mutex_queue);
    pthread_cond_destroy(&cond_queue);
    pthread_attr_destroy(&attr);

    pthread_exit(NULL);
}
