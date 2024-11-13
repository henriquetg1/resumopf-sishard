#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Verifica se foram passados pelo menos 2 argumentos
    if (argc < 3) {
        fprintf(stderr, "Erro: forneça pelo menos 2 argumentos.\n");
        return 1;
    }

    double soma = 0.0;

    // Itera sobre os argumentos
    for (int i = 1; i < argc; i++) {
        double valor = atof(argv[i]); // Converte argumento para número
        soma += valor; // Soma o valor (se não for número, atof retorna 0)
    }

    printf("Soma: %.2f\n", soma);
    return 0;
}
