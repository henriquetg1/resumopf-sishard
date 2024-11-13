#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t a, b, d, sync_b_d, sync_a_f;

void *thread1(void *_arg)
{
    printf("A\n");
    sem_post(&a); // Signal that A has completed for F to proceed
    return NULL;
}

void *thread2(void *_arg)
{
    sem_wait(&sync_a_f); // Wait for F to start
    sem_wait(&a); // Wait for A to complete

    printf("B\n");
    sem_post(&b); // Signal that B has completed
    sem_wait(&sync_b_d); // Wait for D to complete

    printf("C\n");
    return NULL;
}

void *thread3(void *_arg)
{
    sem_wait(&sync_a_f); // Wait for F to start

    printf("D\n");
    sem_post(&d); // Signal that D has completed
    sem_post(&sync_b_d); // Signal B and D synchronization for C and E
    sem_wait(&b); // Wait for B to complete

    printf("E\n");
    return NULL;
}

void *thread4(void *_arg)
{
    printf("F\n");
    sem_post(&sync_a_f); // Signal that F has started (for both B and D to proceed)

    sem_post(&sync_a_f); // Allow Thread 2 and Thread 3 to synchronize with F starting
    sem_wait(&d); // Wait for D to complete

    printf("G\n");
    return NULL;
}

int main(){

    pthread_t tid1, tid2, tid3, tid4;

    sem_init(&a, 0, 0);
    sem_init(&b, 0, 0);
    sem_init(&d, 0, 0);
    sem_init(&sync_b_d, 0, 0);
    sem_init(&sync_a_f, 0, 0);

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
    sem_destroy(&sync_b_d);
    sem_destroy(&sync_a_f);

    return 0;
}
