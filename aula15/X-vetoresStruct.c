#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Definindo a estrutura para armazenar os dois inteiros
typedef struct {
    int valor1;
    int valor2;
} Argumentos;

// Função que cada thread irá executar
void *tarefa_print_i(void *arg) {
    // Fazendo cast de void* para ponteiro para Argumentos
    Argumentos *args = (Argumentos *)arg;
    printf("Valor 1: %d, Valor 2: %d\n", args->valor1, args->valor2);
    return NULL;
}

int main() {
    // Alocando memória para o vetor de threads tids
    pthread_t *tids = (pthread_t *)malloc(4 * sizeof(pthread_t));
    if (tids == NULL) {
        fprintf(stderr, "Falha na alocação de memória para tids\n");
        return 1;
    }

    // Criando 4 estruturas Argumentos, uma para cada thread
    Argumentos *args[4];
    for (int j = 0; j < 4; j++) {
        args[j] = (Argumentos *)malloc(sizeof(Argumentos));
        if (args[j] == NULL) {
            fprintf(stderr, "Falha na alocação de memória para args[%d]\n", j);
            // Liberar qualquer memória previamente alocada
            for (int k = 0; k < j; k++) {
                free(args[k]);
            }
            free(tids);
            return 1;
        }
        // Inicializando os valores da estrutura
        args[j]->valor1 = j + 1;       // Apenas exemplo: valor1 é 1, 2, 3, 4...
        args[j]->valor2 = (j + 1) * 10; // Exemplo: valor2 é 10, 20, 30, 40...
    }

    // Criando as threads e passando a estrutura correspondente como argumento
    for (int j = 0; j < 4; j++) {
        if (pthread_create(&tids[j], NULL, tarefa_print_i, args[j]) != 0) {
            fprintf(stderr, "Erro ao criar a thread %d\n", j);
            // Liberando a memória alocada para argumentos e tids
            for (int k = 0; k < 4; k++) {
                free(args[k]);
            }
            free(tids);
            return 1;
        }
    }

    // Esperando que todas as threads terminem
    for (int j = 0; j < 4; j++) {
        pthread_join(tids[j], NULL);
    }

    // Liberando a memória alocada para cada argumento e para o vetor de threads
    for (int j = 0; j < 4; j++) {
        free(args[j]);
    }
    free(tids);

    return 0;
}
