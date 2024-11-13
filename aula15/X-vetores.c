#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Função que cada thread irá executar
void *tarefa_print_i(void *arg) {
    // usada para acessar o valor que foi passado como argumento para a função da thread:
    int *i = (int *)arg; // Inicializando o ponteiro i com o argumento passado
    printf("Valor de i: %d\n", *i);
    return NULL;
}

int main() {
    // Alocando memória para o vetor de inteiros vi
    int *vi = (int *)malloc(4 * sizeof(int));
    if (vi == NULL) {
        return 1; // Retorna erro se a alocação falhar
    }

    // Alocando memória para o vetor de threads tids
    pthread_t *tids = (pthread_t *)malloc(4 * sizeof(pthread_t));
    if (tids == NULL) {
        free(vi); // Libera vi em caso de falha na alocação de tids
        return 1;
    }

    // Criando 4 threads e armazenando os IDs em tids
    for (int j = 0; j < 4; j++) {
        if (pthread_create(&tids[j], NULL, tarefa_print_i, &vi[j]) != 0) {
            fprintf(stderr, "Erro ao criar a thread %d\n", j);
            free(vi);
            free(tids);
            return 1;
        }
    }

    // Esperando que todas as threads terminem
    for (int j = 0; j < 4; j++) {
        pthread_join(tids[j], NULL);
    }

    // Liberando a memória alocada
    free(vi);
    free(tids);

    return 0;
}
