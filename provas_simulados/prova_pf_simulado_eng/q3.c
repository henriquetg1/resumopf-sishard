#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

char str[100];   // Nome do arquivo a ser processado
int linha_proc = 1;  // Variável global para contar a última linha processada

void sig_handler(int signum) {
    int fd1 = open("q3_status.txt", O_WRONLY | O_CREAT | O_TRUNC, 0700);
    char buf[1];

    int contador = 0;
    int contador_ = 0;
    char *texto = "LINHA_PROC=";
    char num[100];
    sprintf(num, "%d", linha_proc);
    while (1) {
        char char_atual = texto[contador];

        write(fd1, &char_atual, 1);
        if(contador >= 10){
            while(1){
                char num_atual = num[contador_];
                if(num_atual == '\0'){
                    break;
                }
                write(fd1, &num_atual, 1);
                contador_++;

            }
            break;
        }

        contador++;

    }

    close(fd1);
    exit(signum);
}

void conta_linha(char *nome_arquivo) {
    int fd = open(nome_arquivo, O_RDONLY);
    
    if (fd == -1) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }

    char buf[1];
    while (read(fd, buf, 1) > 0) {
        if (buf[0] == '\n') {
            linha_proc++;
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <nome_arquivo>\n", argv[0]);
        exit(1);
    }
    printf("Meu pid: %d\n", getpid());


    struct sigaction s;
    s.sa_handler = sig_handler;
    sigemptyset(&s.sa_mask);
    s.sa_flags = 0;
    sigaction(SIGINT, &s, NULL);
    sigaction(SIGTERM, &s, NULL);

    // Copiar o nome do arquivo para a variável global
    snprintf(str, sizeof(str), "%s", argv[1]);

    // Contar as linhas no arquivo
    conta_linha(str);
    printf("%d", linha_proc);

    return 0;
}
