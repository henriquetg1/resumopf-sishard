#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int num_filhos;

    // Leitura do número de filhos a serem criados
    printf("Digite o número de filhos a serem criados: ");
    scanf("%d", &num_filhos);

    for (int i = 1; i <= num_filhos; i++) {
        pid_t filho = fork();

        if (filho == 0) { // Código executado pelo processo filho
            sleep(5); // Espera de 5 segundos
            printf("Filho número %d: meu PID é %d, o PID do meu pai é %d\n", i, getpid(), getppid());
            return 0; // Filho termina retornando 0
        } else if (filho < 0) {
            perror("Erro ao criar processo filho");
            return 1; // Termina o programa em caso de erro no fork
        }
    }

    // Processo pai espera que todos os filhos terminem
    for (int i = 0; i < num_filhos; i++) {
        wait(NULL);
    }

    return 0;
}
