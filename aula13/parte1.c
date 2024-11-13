#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h> // Inclui strsignal

int main() {
    pid_t pid;
    int wstatus;

    pid = fork(); // Cria um novo processo

    if (pid == -1) {
        perror("Erro no fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Código do processo filho
        printf("Processo filho em execução com PID %d\n", getpid());
        exit(0); // Termina o processo filho com status 0
    } else {
        // Código do processo pai
        pid_t child_pid = wait(&wstatus); // Espera o término do processo filho
        printf("Filho com PID %d acabou\n", child_pid);

        // Verifica se o processo filho terminou normalmente
        if (WIFEXITED(wstatus)) {
            printf("Terminou normal?: %d\n", WIFEXITED(wstatus));
            printf("Valor de retorno: %d\n", WEXITSTATUS(wstatus));
        }

        // Verifica se o processo filho foi terminado por um sinal
        if (WIFSIGNALED(wstatus)) {
            printf("Terminou por sinal?: %d\n", WIFSIGNALED(wstatus));
            printf("Sinal recebido: %d (%s)\n", WTERMSIG(wstatus), strsignal(WTERMSIG(wstatus)));
        }
    }

    return 0;
}
