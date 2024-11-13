#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// NÃO CONTROLAMOS A ORDEM DE EXECUÇÃO DAS THREADS!!!


// Funções que cada thread irá executar
void *print1(void *arg) {
    printf("Thread 1\n");
    return NULL;
}

void *print2(void *arg) {
    printf("Thread 2\n");
    return NULL;
}

void *print3(void *arg) {
    printf("Thread 3\n");
    return NULL;
}

void *print4(void *arg) {
    printf("Thread 4\n");
    return NULL;
}

int main() {
    pthread_t threads[4];

    // Criando as threads
    pthread_create(&threads[0], NULL, print1, NULL);
    pthread_create(&threads[1], NULL, print2, NULL);
    pthread_create(&threads[2], NULL, print3, NULL);
    pthread_create(&threads[3], NULL, print4, NULL);

    // Esperando todas as threads terminarem
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
