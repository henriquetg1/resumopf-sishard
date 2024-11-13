#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int numero;

    // Leitura do número
    printf("Digite um número inteiro: ");
    scanf("%d", &numero);

    // Criação do primeiro processo filho
    pid_t filho1 = fork();

    if (filho1 == 0) { // Processo filho 1
        printf("Processo filho 1: o dobro de %d é %d\n", numero, numero * 2);
        return 0;
    }

    // Criação do segundo processo filho
    pid_t filho2 = fork();

    if (filho2 == 0) { // Processo filho 2
        printf("Processo filho 2: a metade de %d é %.1f\n", numero, numero / 2.0);
        return 0;
    }

    // Processo pai espera os dois filhos terminarem
    wait(NULL);
    wait(NULL);

    return 0;
}
