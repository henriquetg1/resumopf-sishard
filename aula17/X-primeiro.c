#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

// Declaração dos semáforos
sem_t sem_thread1;
sem_t sem_thread2;

// Função que cada thread irá executar
void *tarefa_thread1(void *arg) {
    printf("Thread 1: Antes do ponto de encontro\n");

    // Sinaliza que a thread 1 chegou ao ponto de encontro
    sem_post(&sem_thread1);

    // Espera a sinalização da thread 2
    sem_wait(&sem_thread2);

    printf("Thread 1: Depois do ponto de encontro\n");
    return NULL;
}

void *tarefa_thread2(void *arg) {
    printf("Thread 2: Antes do ponto de encontro\n");

    // Sinaliza que a thread 2 chegou ao ponto de encontro
    sem_post(&sem_thread2);

    // Espera a sinalização da thread 1
    sem_wait(&sem_thread1);

    printf("Thread 2: Depois do ponto de encontro\n");
    return NULL;
}

int main() {
    pthread_t tid1, tid2;

    // Inicialização dos semáforos com valor 0
    sem_init(&sem_thread1, 0, 0);
    sem_init(&sem_thread2, 0, 0);

    // Criação das threads
    pthread_create(&tid1, NULL, tarefa_thread1, NULL);
    pthread_create(&tid2, NULL, tarefa_thread2, NULL);

    // Espera as threads terminarem
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // Destruição dos semáforos
    sem_destroy(&sem_thread1);
    sem_destroy(&sem_thread2);

    return 0;
}
