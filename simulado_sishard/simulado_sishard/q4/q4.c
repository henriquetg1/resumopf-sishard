// Leia o enunciado antes de iniciar este exercício!

// inclua libs aqui!

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

    // Espera por uma qtde aleatória de segundos
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
    t_arg *arg = _arg;

    arg->res = classifier(arg->palavra);
}

/* Função que lê uma palavra de um arquivo.
 * Considere que cada linha do arquivo contem apenas uma palavra.
 *
 * Entradas:
 *   int fd: descritor do arquivo
 *
 * Saída:
 *   Ponteiro (char *) para uma string com a palavra lida (sem \n). Caso o arquivo termine, retorne NULL.
 */
char *read_line(int fd)
{
    // Seu código aqui!
    return NULL;
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

    int fd1 = open(file_name, O_RDONLY);

    if (fd1 == -1)
    {
        fprintf(stderr, "Falha ao abrir o arquivo!\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    int threads_limit = 2; // Vamos começar alocando espaço para 02 threads
    pthread_t *tids = malloc(sizeof(pthread_t) * threads_limit);
    t_arg **args = malloc(sizeof(t_arg *) * threads_limit);

    int threads_created = 0; // Quantas threads criei de fato

    for (int i = 0;; i++)
    {
        // Tenta ler uma palavra
        char *palavra = read_line(fd1);

        // Para quando não conseguir mais ler
        if (palavra == NULL)
        {
            break;
        }

        t_arg *arg = malloc(sizeof(t_arg));
        // Passa a palavra para a thread
        arg->palavra = palavra;

        // Realoca se necessário
        if (threads_limit == threads_created)
        {
            threads_limit *= 2;
            tids = realloc(tids, sizeof(pthread_t) * threads_limit);
            args = realloc(args, sizeof(t_arg *) * threads_limit);
        }

        args[i] = arg;

        pthread_create(&tids[i], NULL, classifier_thread, arg);
        printf("CRIOU THREAD %02d PARA PROCESSAR A PALAVRA %s\n", i, arg->palavra);

        threads_created++;
    }

    // Complete a main!

    return 0;
}

    return 0;
}
