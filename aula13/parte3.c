#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>   // Para a chamada kill
#include <string.h>   // Para strsignal

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
        
        // Entra em loop infinito
        while (1) {
            // Mantém o filho em execução indefinidamente
        }
    } else {
        // Código do processo pai
        printf("Processo pai esperando 10 segundos antes de enviar SIGKILL ao filho\n");
        sleep(10); // Espera por 10 segundos

        printf("Enviando SIGKILL para o processo filho com PID %d\n", pid);
        kill(pid, SIGKILL); // Envia o sinal SIGKILL para o processo filho

        // Espera o término do processo filho e captura o status
        pid_t child_pid = wait(&wstatus);
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
