// Leia o PDF ou MD antes de iniciar este exercício!

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

int main(int argc, char *argv[]){
    // argc diferente de 3
    if (argc != 3) {
        printf("Erro! Número de argumentos inválido! \n");
        fflush(stdout);
        return 1;
    }

    while(1){
        pid_t filho;
        filho = fork();

        if(filho == 0){
            char *origem = argv[1];
            char *destino = argv[2];
            int valor = atoi(argv[3]);
            char str[100];
            sprintf(str, "%d", valor);
            char *args[] = {"./siscoin",origem, destino, str, NULL};
            execvp(args[0], args);

        }
        else if (filho > 0){
            int status;
            waitpid(filho, &status, 0);
            if (WIFEXITED(status)) {
                int acabou = WEXITSTATUS(status);
                if (acabou == 0) {
                    printf("Transferencia realizada! \n");
                    fflush(stdout);
                    break;
                }
                else if(acabou>=1 &&  acabou <= 4){
                    printf("Falha na rede! \n");
                }
                else if(acabou== -1){
                    printf("Erro! Valor inválido. Não pode ser zero nem negativo! \n");
                    fflush(stdout);
                    break;
                }
            } 
        }
    }

    return 0;
}