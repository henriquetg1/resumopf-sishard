// Leia o PDF ou MD antes de iniciar este exercício!


// Faça os includes necessários aqui
// #include ...
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t a, b, c, d;


void *thread1(void *_arg) {

    printf("A\n");
    sem_post(&a);
    sem_post(&a);
    sem_post(&a);

    sem_wait(&b);
    sem_wait(&c);
    sem_wait(&d);

    printf("E\n");

    return NULL;
}

void *thread2(void *_arg) {
    sem_wait(&a);
    printf("B\n");
    sem_post(&b);


    return NULL;
}

void *thread3(void *_arg) {
    sem_wait(&a);

    printf("C\n");
    sem_post(&c);


    return NULL;
}

void *thread4(void *_arg) {
    sem_wait(&a);
    printf("D\n");

    sem_post(&d);

    return NULL;
}

int main(int argc, char *argv[]) {

    // Crie TODAS as threads em um laço. Voce deve utilizar semaforos para sincronizacao.
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;

    sem_init(&a, 0, 0);
    sem_init(&b, 0, 0);
    sem_init(&d, 0, 0);
    sem_init(&c, 0, 0);

    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);
    pthread_create(&tid3, NULL, thread3, NULL);
    pthread_create(&tid4, NULL, thread4, NULL);


    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);

    sem_destroy(&a);
    sem_destroy(&b);
    sem_destroy(&d);
    sem_destroy(&c);




    
    
    return 0;
}