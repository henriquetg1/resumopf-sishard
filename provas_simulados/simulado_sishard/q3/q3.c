#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>

// Variável GLOBAL para armazenar o valor de PI aproximado
double pi = 0.0;

// Função que gera um número aleatório uniformemente entre 0.0 e 1.0
// Você não deve alterar esta função
double random_zero_one() {
    // Gera um número aleatório entre 0 e 1 dividindo o número aleatório
    // gerado por RAND_MAX (valor máximo possível do número aleatório)
    return (double)random() / (double)RAND_MAX;
}

// Função que calcula o valor de pi por simulação (Monte Carlo)
// Você não deve alterar esta função
double aproxima_pi() {
    long dentro = 0;         // Contador para pontos dentro do círculo
    long total_pontos = 0;   // Contador para o total de pontos gerados

    double x, y, d;
    while (1) {
        // Gera coordenadas aleatórias x e y entre 0 e 1
        x = random_zero_one();
        y = random_zero_one();

        // Calcula a distância do ponto (x, y) até a origem (0,0)
        d = x * x + y * y;

        // Verifica se o ponto está dentro do círculo unitário (d <= 1)
        if (d <= 1) {
            dentro++;  // Incrementa o contador de pontos dentro do círculo
        }
        total_pontos++;  // Incrementa o total de pontos gerados

        // Calcula a aproximação de pi com base na razão dos pontos
        pi = 4.0 * ((double)dentro / (double)total_pontos);

        // Pausa o cálculo de vez em quando para simular um processo em andamento
        if (random_zero_one() < 0.008) {
            sleep(1);  // Dorme por 1 segundo
        }
    }
}

// Crie AQUI a função que exporta o valor de pi para um arquivo chamado pi.txt
// Esta função deve ser chamada pelo handler quando este for acionado
void cria_arquivo() {
    // Abre (ou cria) o arquivo pi.txt com permissão de escrita, criação e truncamento
    int fd1 = open("pi.txt", O_WRONLY | O_CREAT | O_TRUNC, 0700);
    char buffer[50000];  // Buffer para armazenar a string que representa o valor de pi

    // Converte o valor de pi em uma string e armazena no buffer
    int length = snprintf(buffer, sizeof(buffer), "%f\n", pi);

    // Escreve o valor de pi no arquivo
    if (write(fd1, buffer, length) == -1) {
        perror("Erro ao escrever no arquivo");  // Mensagem de erro em caso de falha
    }
    close(fd1);  // Fecha o arquivo

    exit(0);  // Encerra o programa após salvar o valor de pi
}

// Crie AQUI a função que será o handler do sinal
void sig_handler(int signum) {
    // Chama a função cria_arquivo para salvar o valor atual de pi em pi.txt
    cria_arquivo();

    // Configura o comportamento padrão para o sinal SIGINT
    struct sigaction handler;
    handler.sa_handler = SIG_DFL;  // Define o manipulador como padrão (default)
    handler.sa_flags = 0;
    sigemptyset(&handler.sa_mask);

    // Redefine a ação do sinal SIGINT para seu comportamento padrão
    sigaction(SIGINT, &handler, NULL);

    // Envia o sinal SIGINT para o próprio processo para encerrar a execução
    kill(getpid(), SIGINT);
}

int main() {
    // Exibe o PID deste processo
    pid_t pai = getpid();
    printf("%d \n", pai);

    // Registra o handler para o sinal SIGINT
    struct sigaction s;
    s.sa_handler = sig_handler;  // Define sig_handler como manipulador do sinal
    sigemptyset(&s.sa_mask);     // Limpa a máscara de sinais
    s.sa_flags = 0;              // Nenhuma flag adicional
    sigaction(SIGINT, &s, NULL); // Associa o manipulador ao sinal SIGINT

    // Inicializa o gerador de números aleatórios com uma semente baseada no tempo atual
    srand(time(NULL));

    // Inicia o cálculo de pi de forma contínua até que o programa seja interrompido
    aproxima_pi();

    return 0;  // Nunca será alcançado devido ao laço infinito em aproxima_pi
}
