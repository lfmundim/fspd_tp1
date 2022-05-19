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
int curr_tid;
pthread_mutex_t mutex_queue;
pthread_mutex_t mutex_tid;
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

int is_eow(task_descr_t task){
    return task.ms == 0 && task.pid == 0;
}

task_descr_t pop(){
    task_descr_t task = task_queue[0];
    for(int i = 0; i<task_count-1; i++){
        task_queue[i] = task_queue[i+1];
    }
    task_count--;

    return task;
}

void* start_thread(){
    pthread_mutex_lock(&mutex_tid);
    int tid = curr_tid;
    curr_tid++;
    pthread_mutex_unlock(&mutex_tid);

    // anuncia inicio da thread
    printf("TB %d\n", tid);

    while(true){
        // retira descritor de tarefas
        task_descr_t task;
        pthread_mutex_lock(&mutex_queue);
        task = pop();

        // se descritor de tarefas é EOW, termina execucao
        if(is_eow(task)) {
            thread_count--;
            pthread_mutex_unlock(&mutex_queue);
            // anuncia fim da thread
            printf("TE %d\n", tid);
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&mutex_queue);

        processa(&task);

        pthread_mutex_lock(&mutex_queue);
        // se ja ha pelo menos min_threads, termina
        if(task_count < thread_count && thread_count > min_threads) {
            thread_count--;
            pthread_mutex_unlock(&mutex_queue);
            // anuncia fim da thread
            printf("TE %d\n", tid);
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&mutex_queue);
        // espera novas tarefas
        // cond wait
        pthread_cond_wait(&cond_queue, &mutex_queue);
    }
}

void fill_in_EOW(){
    int i;
    task_descr_t eow = {
        .ms = 0,
        .pid = 0
    };
    for(i = 0; i<max_threads; i++){
        task_count++;
        task_queue[task_count+i] = eow;
    }
}

int main(int argc, char *argv[])
{
    min_threads = atoi(argv[argc-2]);
    max_threads = atoi(argv[argc-1]);
    curr_tid = 1;
    task_count = 0;

    pthread_mutex_init(&mutex_queue, NULL);
    pthread_mutex_init(&mutex_tid, NULL);
    pthread_attr_init(&attr);
    pthread_cond_init(&cond_queue, NULL);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // cria threadpool
    pthread_t threads[max_threads];

    // le as task descriptions
    int i = 0;
    while(true){
        int pid, ms;
        scanf("%d %d", &pid, &ms);
        if(pid == EOF){
            // EOF, preenche com max_threads EOW
            fill_in_EOW();
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

    int j;
    for(j = 0; j<max_threads; j++){
        thread_count++;
        pthread_create(&threads[j], &attr, &start_thread, NULL);
    }

    pthread_mutex_destroy(&mutex_queue);
    pthread_mutex_destroy(&mutex_tid);
    pthread_cond_destroy(&cond_queue);
    pthread_attr_destroy(&attr);

    pthread_exit(NULL);
}
