#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Verifica se foi fornecido um argumento
    if (argc != 2) {
        fprintf(stderr, "Erro: forneça exatamente 1 argumento.\n");
        return -1;
    }

    // Converte o argumento para long
    long numero = atol(argv[1]);

    // Verifica se o número é negativo
    if (numero < 0) {
        return -1;
    }

    // Verifica se o número é par ou ímpar
    if (numero % 2 == 0) {
        return 1;
    } else {
        return 0;
    }
}
