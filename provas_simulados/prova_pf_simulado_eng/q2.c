#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

int qt = 0;

int main(int argc, char *argv[]) {


    char *arquivo = argv[1];
    double custo = atof(argv[2]);
    char str[100];

    while (1) {
        int status;
        pid_t f1;
        f1 = fork();

        if (f1 == 0) {
            sprintf(str, "%d", qt);
            char *args[] = {"./ocr", arquivo, str, NULL}; // args é um array de strings que contém os argumentos para o programa a ser executado
            execvp(args[0], args); // execvp executa um programa, substituindo o processo atual
            perror("execvp falhou"); // perror imprime a mensagem de erro associada à última função de sistema que falhou
            exit(1);  
        } 
        
        else if (f1 > 0) {
            // Processo pai espera pelo filho
            waitpid(f1, &status, 0);

            if (WIFEXITED(status)) {
                int acabou = WEXITSTATUS(status);

                if (acabou == 0) {
                    printf("OCR TERMINOU NORMALMENTE!\n");
                    printf("TIVEMOS UM GASTO TOTAL DE %.2f\n", (double) (qt + 1) * custo);
                    fflush(stdout);
                    break;
                }
            } 
            else if (WIFSIGNALED(status)) {
                int sig = WTERMSIG(status);
                const char *signal_description = strsignal(sig);
                printf("OCR TERMINOU COM SINAL: %s\n", signal_description);
                fflush(stdout);

            }
        } 
        qt++;
    }
    return 0;
}
