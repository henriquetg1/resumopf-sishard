#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int num;
    char num_str[20];  // Buffer para armazenar a representação em string do número

    while (1) {
        printf("Digite um número: ");
        scanf("%d", &num);

        // Converte o número para string usando sprintf
        sprintf(num_str, "%d", num);

        // Cria um processo filho
        pid_t pid = fork();

        if (pid == -1) {
            perror("Erro ao criar processo filho");
            return 1;
        } else if (pid == 0) {
            // Processo filho: executa o programa eh_par
            // execl serve para substituir o código do processo filho pelo código do programa eh_par
            execl("./eh_par", "eh_par", num_str, (char *)NULL); // nome do executável, argumentos, NULL pois é o último argumento

            // Se execl falhar
            perror("Erro ao executar eh_par");
            exit(1);
        } else {
            // Processo pai: espera pelo retorno do filho
            int status;
            wait(&status);

            if (WIFEXITED(status)) {
                int retorno = WEXITSTATUS(status);

                // Verifica o valor de retorno do processo filho
                if (retorno == -1) {
                    printf("Número negativo, encerrando o programa.\n");
                    break;
                } else if (retorno == 1) {
                    printf("O número %d é par.\n", num);
                } else if (retorno == 0) {
                    printf("O número %d é ímpar.\n", num);
                }
            } else {
                printf("Erro: o processo filho não terminou normalmente.\n");
            }
        }
    }

    return 0;
}
