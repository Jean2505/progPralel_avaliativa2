#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define N_LEITORES 5
#define N_ESCRITORES 3

int leitores = 0;
pthread_mutex_t mutex_leitores = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_escritores = PTHREAD_MUTEX_INITIALIZER;

void* leitor(void* arg){
    int id = *(int*)arg;

    while(1){
        pthread_mutex_lock(&mutex_leitores);
        leitores++;
        if (leitores == 1){
            pthread_mutex_lock(&mutex_escritores);
        }
        pthread_mutex_unlock(&mutex_leitores);

        printf("Leitor %d está lendo.\n", id);
        sleep(1);

        pthread_mutex_lock(&mutex_leitores);
        leitores--;
        if(leitores == 0){
            pthread_mutex_unlock(&mutex_escritores);
        }
        pthread_mutex_unlock(&mutex_leitores);

        sleep(1);
    }

    return NULL;
}


void* escritor(void* arg){
    int id = *(int*)arg;

    while(1){
        pthread_mutex_lock(&mutex_escritores);

        printf("Escritor está escrevendo.\n", id);
        sleep(2);

        pthread_mutex_unlock(&mutex_escritores);

        sleep(1);
    }

    return NULL;
}


int main(){
    pthread_t leitores_threads[N_LEITORES], escritores_threads[N_ESCRITORES];
    int ids_leitores[N_LEITORES], ids_escritores[N_ESCRITORES];

    for (int i = 0; i < N_LEITORES; i++){
        ids_leitores[i] = i + 1;
        if (pthread_create(&leitores_threads[i], NULL, leitor, &ids_leitores[i]) != 0){
            perror("Erro ao criar thread de leitor");
            return 1;
        }
    }

    for (int i = 0; i < N_ESCRITORES; i++){
        ids_escritores[i] = i + 1;
        if (pthread_create(&escritores_threads[i], NULL, escritor, &ids_escritores[i]) != 0){
            perror("Erro ao criar thread de escritor");
            return 1;
        }
    }

    for (int i=0; i<N_LEITORES; i++){
        pthread_join(leitores_threads[i], NULL);
    }
    for (int i=0; i<N_ESCRITORES; i++){
        pthread_join(escritores_threads[i], NULL);
    }

    return 0;
}
