#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

sem_t sem;

typedef struct
{
    char *palavra; // palavra a ser classificada
    int res;       // retorno da thread virá aqui!
} t_arg;

#define MAX_LINE_LENGTH 256

/*
 * Função que simula a classificação de uma palavra
 */
int classifier(char *palavra)
{
    int sleep_time = rand() % 1 + 4;

    // Espera por uma quantidade aleatória de segundos
    sleep(sleep_time);

    int soma = 157;

    for (int i = 0; palavra[i] != '\0'; i++)
    {
        soma += palavra[i];
    }

    int res = (soma % 3);

    return res;
}

/*
 * Classificação em Threads!
 */
void *classifier_thread(void *_arg)
{
    sem_wait(&sem);
    t_arg *arg = (t_arg *)_arg;

    arg->res = classifier(arg->palavra);
    sem_post(&sem);
    return NULL;
}

/* Função que lê uma palavra de um arquivo.
 * Considere que cada linha do arquivo contém apenas uma palavra.
 *
 * Entradas:
 *   int fd: descritor do arquivo
 *
 * Saída:
 *   Ponteiro (char *) para uma string com a palavra lida (sem \n). Caso o arquivo termine, retorne NULL.
 */
char *read_line(int fd)
{
    int continua = 1;
    char buf[1];
    int contador = 0;

    char *palavra = malloc(sizeof(char) * MAX_LINE_LENGTH);

    if (!palavra)
        return NULL;

    while (continua)
    {
        continua = read(fd, buf, 1);
        if (continua == 0)
        {
            break;
        }
        if (buf[0] == '\n')
        {
            break;
        }
        palavra[contador] = buf[0];
        contador++;

        if (contador >= MAX_LINE_LENGTH)
        {
            break;
        }
    }

    palavra[contador] = '\0';
    if (palavra[0] == '\0')
    {
        free(palavra);
        return NULL;
    }

    return palavra;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("USAGE:\n./q4 <n_threads> <file_name>\n");
        return 0;
    }

    int n_threads = atoi(argv[1]); // Info da linha de comando
    char *file_name = argv[2];     // Info da linha de comando

    sem_init(&sem, 0, n_threads);

    int fd1 = open(file_name, O_RDONLY);

    if (fd1 == -1)
    {
        fprintf(stderr, "Falha ao abrir o arquivo!\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    pthread_t *tids = malloc(sizeof(pthread_t) * n_threads);
    t_arg **args = malloc(sizeof(t_arg *) * n_threads);

    int threads_created = 0;
    int contador = 0;
    int total_class_0 = 0, total_class_1 = 0, total_class_2 = 0;

    while (1)
    {
        char *palavra = read_line(fd1);

        if (palavra == NULL)
        {
            break;
        }

        t_arg *arg = malloc(sizeof(t_arg));
        if (!arg) {
            fprintf(stderr, "Erro na alocação de memória para t_arg.\n");
            free(palavra);
            break;
        }
        arg->palavra = palavra;

        args[threads_created] = arg;
        pthread_create(&tids[threads_created], NULL, classifier_thread, arg);
        printf("CRIOU THREAD %02d PARA PROCESSAR A PALAVRA %s\n", contador, arg->palavra);
        contador++;
        threads_created++;
        
        if (threads_created == n_threads)
        {
            for (int i = 0; i < n_threads; i++)
            {
                pthread_join(tids[i], NULL);
                printf("THREAD %02d TERMINOU COM O RESULTADO %d\n", contador - n_threads + i, args[i]->res);

                if (args[i]->res == 0)
                    total_class_0++;
                else if (args[i]->res == 1)
                    total_class_1++;
                else if (args[i]->res == 2)
                    total_class_2++;

                free(args[i]->palavra);
                free(args[i]);
            }
            threads_created = 0;
        }

    }


    //termina as threads que faltavam
    for (int i = 0; i < threads_created; i++)
    {
        pthread_join(tids[i], NULL);
        printf("THREAD %02d TERMINOU COM O RESULTADO %d\n", contador - threads_created + i, args[i]->res);

        if (args[i]->res == 0)
            total_class_0++;
        else if (args[i]->res == 1)
            total_class_1++;
        else if (args[i]->res == 2)
            total_class_2++;

        free(args[i]->palavra);
        free(args[i]);
    }

    sem_destroy(&sem);
    free(tids);
    free(args);
    close(fd1);

    printf("RESULTADO:\n");
    printf("TOTAL CLASSE 0: %d\n", total_class_0);
    printf("TOTAL CLASSE 1: %d\n", total_class_1);
    printf("TOTAL CLASSE 2: %d\n", total_class_2);

    return 0;
}
