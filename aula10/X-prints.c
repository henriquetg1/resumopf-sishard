#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;
    int id_programa;

    printf("Eu sou o processo pai, pid=%d, meu id do programa é 0\n", getpid());

    for (id_programa = 1; id_programa <= 8; id_programa++) {
        pid = fork();

        if (pid < 0) { // Verifica erro na criação do processo
            perror("Erro ao criar processo");
            return 1;
        } else if (pid == 0) { // Processo filho
            printf("Eu sou um processo filho, pid=%d, ppid=%d, meu id do programa é %d\n", getpid(), getppid(), id_programa);
            return 0; // Garante filho não crie novos processos
        }
    }

    // Processo pai espera que todos os filhos terminem
    for (int i = 0; i < 8; i++) {
        wait(NULL); // 
    }

    return 0;
}