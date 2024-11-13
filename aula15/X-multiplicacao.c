#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Definindo a estrutura para armazenar os dois inteiros e o resultado
typedef struct {
    int valor1;
    int valor2;
    int resultado; // Campo para armazenar o resultado da multiplicação
} Argumentos;

// Função que cada thread irá executar
void *tarefa_multiplicacao(void *arg) {
    Argumentos *args = (Argumentos *)arg;
    args->resultado = args->valor1 * args->valor2;
    return NULL;
}

int main() {
    pthread_t tids[4];
    Argumentos args[4];

    // Inicializando os valores e criando as threads
    for (int j = 0; j < 4; j++) {
        args[j].valor1 = j + 1;
        args[j].valor2 = (j + 1) * 10;

        pthread_create(&tids[j], NULL, tarefa_multiplicacao, &args[j]);
    }

    // Esperando que todas as threads terminem e imprimindo o resultado
    for (int j = 0; j < 4; j++) {
        pthread_join(tids[j], NULL);
        printf("Thread %d - Resultado da multiplicação: %d\n", j, args[j].resultado);
    }

    return 0;
}
