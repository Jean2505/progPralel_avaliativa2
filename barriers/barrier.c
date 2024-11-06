#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5

typedef struct{
    int count;
    int waiting;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} barrier_t;


void barrier_init(barrier_t *barrier, int count){
    barrier->count = count;
    barrier->waiting = 0;
    pthread_mutex_init(&barrier->mutex, NULL);
    pthread_cond_init(&barrier->cond, NULL);
}


void barrier_wait(barrier_t *barrier){
    pthread_mutex_lock(&barrier->mutex);
    barrier->waiting++;

    if (barrier->waiting == barrier->count){
        barrier->waiting = 0;
        pthread_cond_broadcast(&barrier->cond);
    }
    else {
        pthread_cond_wait(&barrier->cond, &barrier->mutex);
    }

    pthread_mutex_unlock(&barrier->mutex);
}


void* thread_task(void* arg){
    int id = *((int*)arg);
    printf("Thread %d chegou antes da barreira.\n", id);

    barrier_wait((barrier_t*)arg);

    printf("Thread %d passou da barreira.\n", id);
    return NULL;
}


int main(){
    pthread_t threads[NUM_THREADS];
    barrier_t barrier;
    barrier_init(&barrier, NUM_THREADS);

    int thread_ids[NUM_THREADS];
    for (int i=0; i<NUM_THREADS; i++){
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_task, (void*)&barrier);
    }

    for (int i=0; i<NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&barrier.mutex);
    pthread_cond_destroy(&barrier.cond);

    return 0;
}
