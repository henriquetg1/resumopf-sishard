# Resumo Detalhado de Sistemas Operacionais

Este resumo visa aprofundar os conceitos relacionados a processos, carregamento de programas, entrada/saída, sinais e concorrência com threads, incluindo as imagens fornecidas, para auxiliá-lo na preparação para a prova.

---

## **Processos**

### **Funções `wait` e `waitpid`**

As funções `wait` e `waitpid` são utilizadas para sincronizar processos pai e filho em sistemas Unix-like.

```c
pid_t wait(int *wstatus);
pid_t waitpid(pid_t pid, int *wstatus, int options);
```

- **`wait`**:
  - Espera por **qualquer** processo filho terminar.
  - Uso comum quando o pai não precisa saber qual filho terminou.

- **`waitpid`**:
  - Espera por um **filho específico**, identificado pelo `pid`.
  - Permite maior controle sobre qual processo filho o pai está aguardando.

![Função wait e waitpid](images/image-2.png)

![Exemplo de espera de processos](images/image-1.png)

### **Estados dos Processos**

- **Processo Zumbi**: Ocorre quando um processo filho termina, mas o processo pai ainda não chamou `wait` ou `waitpid` para coletar o status de término. O filho permanece na tabela de processos como zumbi.

![Estados dos processos](images/processos.png)

- **Importância de `wait`/`waitpid`**:
  - Evita o acúmulo de processos zumbis.
  - Garante que os recursos usados pelos processos filhos sejam liberados adequadamente.

---

## **Carregamento de Programas**

As funções da família `exec` são usadas para carregar e executar novos programas dentro do processo atual.

- **Argumentos (`args`)**:
  - Devem sempre terminar com `NULL` para indicar o fim da lista de argumentos.

- **Comportamento do `exec`**:
  - Se a chamada ao `execvp` for bem-sucedida, o código que está abaixo dele **não** será executado, pois o processo atual é substituído pelo novo programa.

- **Função `execvp`**:
  - Recebe o caminho para o executável (pode incluir `./` para indicar o diretório atual) e um array de argumentos.

![Uso de execvp](images/image-3.png)

- **Exemplo de Uso**:

```c
char *args[] = {"./programa", "arg1", "arg2", NULL};
execvp(args[0], args);
// Código abaixo não será executado se execvp for bem-sucedido
```

![Fluxo de execução com exec](images/image-4.png)

---

## **Entrada/Saída**

As funções de leitura e escrita em baixo nível permitem manipular dados diretamente através de descritores de arquivo.

- **Funções `read` e `write`**:

```c
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
```

- **Comportamento do `read`**:
  - Retorna o número de bytes efetivamente lidos.
  - Retorna `0` quando chega ao fim do arquivo.
  - Pode **bloquear** o programa até que os dados estejam disponíveis.

![Fluxo de leitura](images/image-5.png)

- **Exemplo de Uso do `read`**:

```c
char buf[100];
ssize_t nbytes = read(fd, buf, sizeof(buf));
if (nbytes > 0) {
    // Processar dados lidos em buf
}
```

![Detalhes da função read](images/image-7.png)

### **Abrindo Arquivos com `open`**

- **Função `open`**:

```c
int open(const char *pathname, int flags, mode_t mode);
```

- **Flags Comuns**:
  - `O_RDONLY`: Abre o arquivo somente para leitura.
  - `O_WRONLY`: Abre somente para escrita.
  - `O_RDWR`: Abre para leitura e escrita.
  - `O_CREAT`: Cria o arquivo se ele não existir.
  - `O_EXCL`: Usado com `O_CREAT`, causa erro se o arquivo já existir.

![Uso de flags em open](images/image-6.png)

- **Exemplo com `O_EXCL`**:
  - Permite lidar com erros ao tentar criar um arquivo que já existe.

![Exemplo de O_EXCL](images/image-12.png)

### **Manipulação de Dados**

- **Acessando Caracteres**:
  - `buf[0]` é o primeiro caractere lido.
  
- **Escrevendo Números como Caracteres**:
  - Para converter um dígito inteiro `i` em seu caractere correspondente: `'0' + i`.
  - Exemplo: Se `i = 5`, `'0' + i` resulta em `'5'`.

![Conversão de inteiros para caracteres](images/image-8.png)

---

## **Enviando Sinais**

Sinais são um mecanismo usado para notificar processos sobre eventos assíncronos.

![Lista de sinais comuns](images/image-13.png)

### **Sinais Comuns**

- **`SIGINT`**: Interrompe o processo (Ctrl+C).
- **`SIGSTOP`**: Suspende a execução do processo.
- **`SIGCONT`**: Continua a execução de um processo suspenso.
- **`SIGKILL`**: Termina o processo imediatamente (não pode ser ignorado).
- **`SIGSEGV`**: Erro de segmentação (acesso inválido à memória).
- **`SIGFPE`**: Erro aritmético (ex.: divisão por zero).

### **Comando `kill`**

- Usado para enviar sinais a processos.

```bash
kill -SIGKILL <PID>
```

- **Sinais e seus Números**:
  - `SIGKILL (9)`: Encerra o processo imediatamente.
  - `SIGTERM (15)`: Solicita que o processo termine graciosamente.
  - `SIGINT (2)`: Interrompe o processo (como Ctrl+C).

---

## **Capturando Sinais**

Para que um processo responda de maneira personalizada a certos sinais, é necessário capturá-los e definir um tratador.

- **Limitções**:
  - Não pode haver mais de um handler do mesmo sinal sendo executado simultaneamente.

![Fluxo de captura de sinais](images/image-15.png)

### **Configurando Tratadores de Sinal**

- **Exemplo de Tratadores**:

```c
int status = 0;

void sigint_handler(int num) {
    status += 1;
    printf("Chamou Ctrl+C; status=%d\n", status);
    operacao_lenta();
    printf("SIGINT: Vou usar status agora! status=%d\n", status);
}

void sigterm_handler(int num) {
    status += 1;
    printf("Recebi SIGTERM; status=%d\n", status);
    operacao_lenta();
    printf("SIGTERM: Vou usar status agora! status=%d\n", status);
}
```

![Configuração de tratadores](images/image-14.png)

- **Considerações**:
  - Ao configurar `sigaction`, podemos definir uma máscara de sinais para bloquear outros sinais enquanto um tratador está em execução.

![Máscara de sinais](images/image-16.png)

- **Exemplo Completo**:

```c
void sigtstp_handler(int num) {
    printf("Programa suspenso com Ctrl+Z\n");

    // Redefine o comportamento padrão de SIGTSTP
    struct sigaction action;
    action.sa_handler = SIG_DFL;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGTSTP, &action, NULL);

    // Envia SIGTSTP ao próprio processo para suspender
    raise(SIGTSTP);
}

void sigcont_handler(int num) {
    printf("Continuando!\n");
}

int main() {
    // Configuração dos tratadores...
    // Código omitido para brevidade
}
```

![Fluxo completo de sinais](images/image-17.png)

---

## **Concorrência e Threads**

Threads permitem a execução concorrente dentro de um mesmo processo, compartilhando o espaço de memória.

![Conceito de threads](images/image-18.png)

### **Características das Threads**

- **Compartilhamento de Memória**:
  - Todas as threads de um processo compartilham o mesmo espaço de endereço.
  - Isso facilita a comunicação, mas requer sincronização para evitar condições de corrida.

- **Escalonamento pelo Kernel**:
  - Threads são escalonadas pelo kernel, e não temos controle sobre a ordem de execução.
  - Devemos programar considerando que qualquer thread pode ser interrompida a qualquer momento.

![Escalonamento de threads](images/image-19.png)

### **Funções de Threads**

- **Criando Threads**:

```c
#include <pthread.h>

void *minha_thread(void *arg) {
    // Código da thread
    return NULL;
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, minha_thread, NULL);
    pthread_join(tid, NULL);
    return 0;
}
```

- **`pthread_create`**:
  - Cria uma nova thread.
  - Recebe a função que a thread irá executar e um argumento para essa função.

- **`pthread_join`**:
  - Aguarda a conclusão de uma thread.
  - Somente uma thread pode chamar `pthread_join` para a mesma thread de destino.

### **Passando Argumentos e Retornando Valores**

- **Passagem de Argumentos**:
  - Use estruturas para passar múltiplos argumentos para uma thread.

- **Exemplo Completo**:

```c
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

// Estrutura para armazenar os argumentos e o resultado das threads
typedef struct {
    int a;
    int b;
    int result; // Campo para armazenar o resultado da multiplicação
} thread_args;

// Função que será executada pelas threads
void *minha_thread(void *arg) {
    thread_args *args = (thread_args *) arg; // Faz o cast do argumento para o tipo struct
    args->result = args->a * args->b;        // Calcula a multiplicação e armazena em result
    printf("Thread com valores a=%d, b=%d, result=%d\n", args->a, args->b, args->result);
    
    return NULL;
}

// Função que cria as threads
pthread_t *criar_threads(int n, thread_args **args_array) {
    pthread_t *tids = malloc(sizeof(pthread_t) * n);

    for (int i = 0; i < n; i++) {
        // Aloca e configura os argumentos para cada thread
        thread_args *args = malloc(sizeof(thread_args));
        args->a = i + 1;   // Exemplo de valor para `a`
        args->b = (i + 1) * 2; // Exemplo de valor para `b`
        args->result = 0;   // Inicializa o resultado com 0
        args_array[i] = args; // Armazena o ponteiro para os argumentos no array

        pthread_create(&tids[i], NULL, minha_thread, args);
    }

    return tids;
}

int main() {
    int n = 4;
    thread_args *args_array[n];
    pthread_t *tids = criar_threads(n, args_array);

    // Aguarda todas as threads terminarem
    for (int i = 0; i < n; i++) {
        pthread_join(tids[i], NULL);
    }

    // Exibe os resultados calculados por cada thread
    printf("Resultados no main:\n");
    for (int i = 0; i < n; i++) {
        printf("Thread %d: %d * %d = %d\n", i, args_array[i]->a, args_array[i]->b, args_array[i]->result);
        free(args_array[i]); // Libera a memória alocada para os argumentos da thread
    }

    free(tids); // Libera a memória alocada para as threads
    return 0;
}
```

## Sincronização com Mutex

![alt text](images/image-21.png)

![alt text](images/image-22.png)

![alt text](images/image-23.png)

Programa que cria um array e soma uma parte dele (de start ate end). Usando Mutex para garantir que enquanto uma thread está somando, as outras esperam a soma acabar, para alterar seu valor.

- **Observação** :
    - Não precisamos usar mutex no exemplo acima.
    - Se fizermos a função retornar a sua soma parcial através de um campo no struct.
    - Podemos fazer a soma após *pthread_join*.

![alt text](images/image-25.png)

![alt text](images/image-24.png)

## Semáforos

![alt text](images/image-26.png)

- **Definição**: duas tarefas podem ser feitas em paralelo se : 

    - elas não compartilham absolutamente nenhuma informação.
    - elas compartilham informação mas possuem mecanismos de sincronização de tal maneira que toda ordem de execução possível de suas instruções resulte no mesmo resultado final.

Semáforos são usados para o 2° caso.

- **Considerações**:
  - Cada thread recebe um conjunto de argumentos exclusivos.
  - Os resultados são armazenados na estrutura compartilhada.
  - A sincronização é necessária se threads compartilharem recursos.

![alt text](images/image-27.png)

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 10
#define MAX 2

typedef struct {
    sem_t *sem;
} arg_thread_t;


void *faz_algo(void *arg_) {
    arg_thread_t *arg = arg_;

    // Wait no semáforo: solicita permissão para executar
    sem_wait(arg->sem);

    printf("Thread executando!\n");

    // simula tarefa
    while (random() > 100);

    printf("Thread vai finalizar!\n");

    // Post no semáforo: avisa que terminou de executar
    sem_post(arg->sem);

    return NULL;
}

int main() {

    // Configura semáforo
    sem_t semaph;

    // Inicializa semáforo com o número máximo de threads executando simultaneamente
    sem_init(&semaph, 0, MAX); 

    // Configura argumentos
    arg_thread_t *arg = malloc(sizeof(arg_thread_t));
    arg->sem = &semaph;

    pthread_t *tid = malloc(sizeof(pthread_t) * N);
    // Cria todas as threads (o semáforo irá controlar a execução)
    for (int i=0; i<N; i++){
        pthread_create(&tid[i], NULL, faz_algo, arg);
    }

    // espere pelas threads
    for (int i=0; i<N; i++){
        pthread_join(tid[i], NULL);
    }

    sem_destroy(&semaph);

    free(tid);
    free(arg);

    return 0;
}
```
---

## **Dicas Adicionais para a Prova**

- **Compreenda os Conceitos Básicos**:
  - Certifique-se de entender como processos e threads funcionam, incluindo a criação, sincronização e comunicação entre eles.

- **Pratique com Código**:
  - Escreva programas que utilizem `fork`, `exec`, manipulação de arquivos e threads.
  - Experimente capturar e enviar sinais em seus programas.

- **Sincronização é Fundamental**:
  - Ao trabalhar com threads, utilize mecanismos como mutexes para evitar condições de corrida.
  - Entenda como bloquear sinais para proteger seções críticas.

- **Estude os Tratadores de Sinais**:
  - Saiba como configurar tratadores usando `sigaction`.
  - Lembre-se das limitações, como não poder ter mais de um handler do mesmo sinal executando simultaneamente.

- **Gerencie Recursos Adequadamente**:
  - Sempre feche descritores de arquivo abertos.
  - Libere memória alocada dinamicamente para evitar vazamentos.

- **Leitura da Documentação**:
  - Utilize o comando `man` para consultar detalhes das funções.
  - Exemplo: `man read` para documentação da função `read`.

---

## **Prova Eng**

### Questão 1

A figura ilustra as relações de dependência entre as partes das funções thread1, thread2, thread3 e thread4, mostrando que algumas delas poderiam ser feitas de maneira concorrente.

Seu trabalho nesta questão será:

Criar threads para execução concorrente das funções. (40% da nota).

Usar semáforos para que a ordem dos prints das partes das tarefas respeitem o diagrama da figura. (60% da nota).

![alt text](images/image-28.png)

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t f, a, b , d, k;


void *thread1(void *_arg)
{
    printf("A\n");
    sem_post(&a);

    return NULL;
}

void *thread2(void *_arg){
    sem_wait(&k);   
    printf("B\n");

    sem_post(&b);
    sem_wait(&d);

    printf("C\n");
    return NULL;
}

void *thread3(void *_arg){

    sem_wait(&k);
    printf("D\n");

    sem_post(&d);
    sem_wait(&b);

    printf("E\n");
    return NULL;
}

void *thread4(void *_arg)
{
    printf("F\n");
    sem_wait(&a);

    sem_post(&k);
    sem_post(&k);

    printf("G\n");

    return NULL;
}

int main(){

    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;

    sem_init(&a, 0, 0);
    sem_init(&b, 0, 0);
    sem_init(&d, 0, 0);
    sem_init(&f, 0, 0);
    sem_init(&k, 0, 0);

    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);
    pthread_create(&tid3, NULL, thread3, NULL);
    pthread_create(&tid4, NULL, thread4, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);

    sem_destroy(&a);
    sem_destroy(&b);
    sem_destroy(&d);
    sem_destroy(&f);
    sem_destroy(&k);

    return 0;
}
```

### Questão 2

Neste exercício, o programa q2 recebe como argumento da linha de comando o nome de um arquivo de imagem e um preço por chamada de API. Sua tarefa é chamar outro programa, que simula um reconhecimento de textos contidos na imagem, e calcular o gasto total com as chamadas da API.

Exemplo de como seu programa q2 será chamado:
```c
./q2 cnh_do_joao.png 0.1
```

Uma tarefa comum em programação de sistemas é integrar com outros programas instalados. Assim sendo, a imagem (no exemplo é cnh_do_joao.png) deverá ser examinada pelo programa ocr, disponível de forma compilada para x86 na pasta da questão.

O executável ocr tem a chamada no padrão ./ocr arquivo 0 onde arquivo representa um arquivo de imagem e 0 representa quantas vezes você já tentou processar este arquivo. Assim sendo, na primeira vez que o ocr é chamado para processar o arquivo jose.jpg devemos utilizar ./ocr jose.jpg 0. Caso o processo falhe, então tentamos novamente usando ./ocr jose.jpg 1. Se falhar mais uma vez, o uso deve ser ./ocr jose.jpg 2 e assim sucessivamente até que ele saia com sucesso.

O programa ocr retorna 0 quando a chamada é feita com sucesso, ou -1 quando recebe uma chamada errada (quantidade incorreta de argumentos). Você não precisa se preocupar com o que de fato o programa ocr faz, apenas considere que a ele deve processar a imagem e retornar um resultado (a imagem sequer deve existir, é tudo simulado).

Entretanto, um problema é que as vezes as chamadas ao programa ocr são interrompidas devido a sinais recebidos (ele foi programado por um péssimo programador). Assim, você deve trabalhar no arquivo q2.c para:

Criar um processo filho e nele, executar o ocr, passando para o ocr o primeiro argumento recebido pelo executável q2 (que é a imagem) e um inteiro com a quantidade de vezes que você já tentou processar esta imagem (na primeira vez, passe 0, se tiver sinal, passe

1). No proceso pai: Caso o processo filho TENHA finalizado normalmente, exiba a mensagem OCR TERMINOU NORMALMENTE!\n (10% da nota).
Caso o processo filho TENHA SIDO sinalizado, exiba a mensagem OCR TERMINOU COM SINAL %s\n, substituindo %s pelo texto que descreve o sinal. (40% da nota)
Repita a chamada do ocr (primeiro passo) até que ele finalize normalmente. No final, imprima o gasto total (double) no padrão TIVEMOS UM GASTO TOTAL DE %.2f\n. Aqui, lembre de atualizar em mais um a quantidade de vezes que você já tentou processar esta imagem (50% da nota)
Atenção:

Imprima EXATAMENTE conforme solicitado, sem deixar espaços antes ou depois das frases. Dê apenas um \n ao final. Por via das dúvidas, utilize fflush(stdout).

O gasto total é calculado pela quantidade de vezes que é preciso chamar o ocr multiplicada pelo preço por chamada de API.

Trabalhe no arquivo q2.c e complete as partes faltantes

Compile com gcc -g q2.c -o q2

Deixei um arquivo executável q2_ref que exemplifica o comportamento desejado do programa final (é um gabarito!). Teste, por exemplo, com ./q2_ref escrita.png 2.5 e utilize como referência!

O executável ocr tem comportamento determinístico. Se você programar corretamente e passar os argumentos corretamente para o ocr, deve obter as mesmas saídas que o q2_ref para as mesmas entradas.

Nesta questão você deverá usar as funções de gerenciamento de processos e chamada de executáveis vistas em aula. Você não pode, por exemplo, usar system.

```c
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
            char *args[] = {"./ocr", arquivo, str, NULL};
            execvp(args[0], args);
            perror("execvp falhou");
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
```

### Questão 3

Abra o arquivo q3.c!

Esta aplicação simula o processamento sequencial de diversas linhas, onde cada linha representa uma tarefa.

Uma nova funcionalidade foi requisitada nesta aplicação e você é o responsável por implementá-la: queremos poder guardar o status atual do processamento em algum arquivo para poder continuar o processamento uma outra hora.

Seu objetivo neste exercício:

Ao receber um sinal SIGTERM ou SIGINT, antes de finalizar, criar um arquivo q3_status.txt contendo em seu conteúdo informações sobre a última linha processada seguida de um '\n'. Ex de conteúdo do arquivo a ser gerado: LINHA_PROC=15. (60% da nota pelo registro do handler e pela mensagem no arquivo)

O programa sai com o mesmo sinal que recebeu. (20% da nota)

Se o arquivo já existir, ele deve ser sobrescrito. (20% da nota)

OBS:

Você deve criar as funções para serem handlers dos sinais

Os Handlers devem ser registrados na main e serem auto-contidos.

Para os arquivos, utilize APENAS as chamadas vistas em aula: open, close, read, write. Não pode utilizar fopen, fgets, fscanf por exemplo. Se utilizar, a nota do ex será zero (mesmo que passe nos testes).

Para testar, envie sinais pelo terminal e confira o arquivo de log gerado!

O handler deve utilizar a variável global linha_proc para saber a última linha processada.

Não iremos continuar a tarefa a próxima vez que rodarmos o executável, estamos apenas simulando! Queremos salvar onde paramos para poder continuar, mas na verdade não iremos continuar (uma coisa de cada vez!)

```c
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
```

### Questão 4

Neste exercício, vamos fazer o reconhecimento de CEPs em um arquivo de textos, dividindo a carga de trabalho entre threads!

O programa q4 recebe como argumento da linha de comando uma quantidade total de threads a serem criadas e o nome de um arquivo.

Exemplo de como seu programa q4 será chamado:

./q4 2 arquivo.txt
onde 2 é a quantidade de threads a serem criadas e arquivo.txt é o arquivo que contem em uma única linha palavras a serem processadas pelas threads.

Exemplo de conteúdo do arquivo.txt:

eu moro no cep numero 70901-001 mas trabalho no 18900-199 longe demais
Perceba que na main já são criadas as threads da função cep_validation_thread. A thread recebe como argumento uma estrutura que contém o descritor de um arquivo (que é compartilhado entre todas as threads existentes). Cada thread executa o seguinte algoritmo em alto nível:

REPITA:
Tenta ler uma palavra do arquivo compartilhado:
SE não conseguiu ler:
PARE
SE a palavra lida for um CEP, ENTÃO:
SOMA 1 na variável count_ceps
Boa parte do código já está desenvolvido. Sua primeira tarefa será programar a função char *read_word(int fd), que tem por objetivo ler uma palavra de um arquivo.

Assim que tiver feito esta etapa, você irá perceber que boa parte do código já irá funcionar.

Tente compilar com:

gcc -g q4.c -o q4 -pthread
E executar com apenas uma thread:

./q4 1 in01.txt
Você verá que a resposta gerada estará correta. Entretanto, ao executar com arquivos maiores e mais threads, perceba que o resultado deixará de ser consistente. Tente rodar várias vezes o código:

./q4 8 in03.txt
Então, sua tarefa envolverá tratar corretamente as regiões críticas.

Perceba que, diferente do lab de threads, aqui não existe uma pré-definição de quais palavras devem ser tratadas por quais threads!

Suas tarefas:

Programar a função char *read_word(int fd). Considere que o arquivo já está aberto e deve ser fechado apenas na main. Para ler, é obrigatório utilizar apenas a chamada read. (40% da nota)

Fazer alterações na main e na função cep_validation_thread para que as regiões críticas sejam tratadas adequadamente. O código deve ter sua funcionalidade geral mantida (continuar o mesmo número de threads, continuar a cada thread poder ler uma palavra indefinida do arquivo) e apenas estruturas de sincronização devem ser adicionadas para tratar as regiões críticas. Se usar variável global, haverá desconto. (40% da nota).

Programa passa no valgrind sem erros. Aqui, também será necessário liberar memórias alocadas na main no código que receberam pronto! (20% da nota, apenas se resolveu corretamente os anteriores)

OBS:

Trabalhe no arquivo q4.c e complete as partes faltantes

Compile com gcc -g q4.c -o q4 -pthread

Faça seus próprios arquivos de teste personalizados!

Se utilizar variáveis globais, haverá desconto.

```c
// Leia o PDF ou MD antes de iniciar este exercício!

// inclua libs aqui!
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

char *read_word(int fd);

typedef struct
{
    int id_thread;   // identificador da thread
    int *count_ceps; // para as threads saberem onde guardar o resultado!
    int file_desc;   // descritor do arquivo
    pthread_mutex_t *mutex_file;
} t_arg;

/*
 * Função que verifica se um char é numérico
 */
int isnum(char c)
{
    return c >= '0' && c <= '9';
}

/*
 * Função que verifica se uma palavra é um cep válido
 * do ponto de vista do formado DDDDD-DDD
 * onde D é um dígito
 *
 * ENTRADA:
 * char *txt: cep a ser validado
 *
 * SAIDA:
 *   (int) 0: o CEP é válido
 *   (int) 1: Tamanho da palavra não é tamanho de CEP!
 *   (int) 2: Tamanho ok mas formato inválido!
 */
int validade_cep(char *txt)
{
    char *p;
    int valid = 1;
    int qtde = 0;
    for (qtde = 0; txt[qtde] != '\0'; qtde++)
        ;
    if (qtde != 9)
    {
        return 1;
    }
    if (isnum(txt[0]) &&
        isnum(txt[1]) &&
        isnum(txt[2]) &&
        isnum(txt[3]) &&
        isnum(txt[4]) &&
        txt[5] == '-' &&
        isnum(txt[6]) &&
        isnum(txt[7]) &&
        isnum(txt[8]))
    {
        return 0;
    }
    else
    {
        return 2;
    }
}

/*
 * Faz validação de cep em Thread.
 *
 * A thread recebe um descritor de arquivo e deve ler
 * palavras dele enquanto o arquivo não acabar!
 * 
 */
void *cep_validation_thread(void *_arg)
{
    t_arg *arg = _arg;

    while (1)
    {
        // Tenta ler uma palavra do arquivo
        pthread_mutex_lock(arg->mutex_file);
        char *palavra = read_word(arg->file_desc);
        pthread_mutex_unlock(arg->mutex_file);
        if (palavra == NULL)
        {
            // FUNCAO read_word NAO IMPLEMENTADA OU FINAL DO ARQUIVO!
            break;
        }

        printf("THREAD %02d VAI PROCESSAR PALAVRA [%s]\n", arg->id_thread, palavra);
        fflush(stdout);

        int ret = validade_cep(palavra);
        free(palavra);
        if (ret == 0)
        {
            *arg->count_ceps = *arg->count_ceps + 1;
        }
    }
}

/* Função que lê uma palavra de um arquivo.
 * Considere que o arquivo contém apenas uma linha, sem \n no final.
 * A linha possui diversas palavras, separadas por um único espaço
 *
 * Entradas:
 *   int fd: descritor do arquivo
 *
 * Saída:
 *   Ponteiro (char *) para uma string com a palavra lida. Caso o arquivo termine, retorne NULL.
 */
char *read_word(int fd)
{
    char c;
    int bytes;
    char *retval = malloc(100 * sizeof(char));
    int i = 0;
    while (1)
    {
        bytes = read(fd, &c, 1);
        if ((bytes == 0) && (i == 0))
        {
            free(retval);
            return NULL;
        }
        if ((c == ' ') || (bytes == 0))
        {
            retval[i] = '\0';
            return retval;
        }
        retval[i] = c;
        i++;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("USAGE:\n./q4 <n_threads> <file_name>\n");
        return EXIT_FAILURE;
    }

    int n_threads = atoi(argv[1]); // Info da linha de comando
    char *file_name = argv[2];     // Info da linha de comando

    int fd1 = open(file_name, O_RDONLY); // Abre o arquivo

    if (fd1 == -1)
    {
        fprintf(stderr, "Falha ao abrir o arquivo!\n");
        exit(EXIT_FAILURE);
    }

    // Inicializa contador de CEPs encontrados
    int count_ceps = 0;

    // Aloca espaço para tids e args das threads
    pthread_t *tids = malloc(sizeof(pthread_t) * n_threads);
    t_arg **args = malloc(sizeof(t_arg *) * n_threads);
    pthread_mutex_t mutex_file = PTHREAD_MUTEX_INITIALIZER;

    for (int i = 0; i < n_threads; i++)
    {

        t_arg *arg = malloc(sizeof(t_arg));
        arg->file_desc = fd1; // Thread recebe o file descriptor
        arg->count_ceps = &count_ceps;
        arg->id_thread = i;
        arg->mutex_file = &mutex_file;
        args[i] = arg;

        pthread_create(&tids[i], NULL, cep_validation_thread, arg);
        printf("CRIOU THREAD %02d\n", i);
    }

    for (int i = 0; i < n_threads; i++)
    {
        pthread_join(tids[i], NULL);
        free(args[i]);
        printf("THREAD %02d TERMINOU\n", i);
    }

    printf("RESULTADO:\nFORAM ENCONTRADOS %02d CEPS VALIDOS\n", count_ceps);

    close(fd1);
    free(args);
    free(tids);

    return 0;
}
```
---
```c

```
---

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int exit_status;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso incorreto! Passar nome de imagem como argumento.\n");
        return EXIT_FAILURE;
    }

    char *imagem = argv[1];
    int tentativas = 1;
    int chamadas_api = 0;

    while (1) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("Erro ao criar o processo");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            char tentativa_str[10];
            snprintf(tentativa_str, sizeof(tentativa_str), "%d", tentativas);
            execlp("./digitaliza", "digitaliza", imagem, tentativa_str, NULL);
            perror("Erro ao executar o digitaliza");
            exit(EXIT_FAILURE);
        } else {
            int status;
            chamadas_api++;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status)) {
                exit_status = WEXITSTATUS(status);
                printf("digitaliza TERMINOU NORMALMENTE!\n");
                break;
            } else if (WIFSIGNALED(status)) {
                int signal_number = WTERMSIG(status);
                printf("digitaliza TERMINOU COM SINAL %s\n", strsignal(signal_number));
                tentativas++;
            }
        }
    }

    printf("TIVEMOS %d CHAMADAS DA API\n", chamadas_api);
    return 0;
}
```
---
```c

// Leia o HTML ou MD antes de iniciar este exercício!

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

// Variável Global para o handler saber até qual imagem processou
int ultima_imagem = 0;


// Função que gera um numero aleatorio uniformemente entre 0.0 e 1.0
// Você não deve alterar esta função
double random_zero_one()
{
    return (double)random() / (double)RAND_MAX;
}

void simulacao_download()
{
    if (random_zero_one() < 0.5)
    {
        sleep(1);
    }
}

int baixa_imagens(int qtde_imagens)
{
    for (ultima_imagem = 0; ultima_imagem < qtde_imagens; ultima_imagem++)
    {
        simulacao_download();
        printf("BAIXOU A IMAGEM %d\n", ultima_imagem);
        fflush(stdout);
    }
}

// Crie AQUI a função que exporta o valor de `ultima_imagem` para um arquivo chamado q3_status.txt
// O arquivo deve ter apenas uma linha contendo, ULTIMA_IMAGEM seguido de um sinal de igual e
// seguido da última linha processada, seguido de \n:
// ULTIMA_IMAGEM=15
//
// Esta função deve ser chamada pelo handler quando este for acionado

// Crie AQUI a função que será o handler do sinal
void sig_handler(int signum) {
    int fd1 = open("q3_status.txt", O_WRONLY | O_CREAT | O_TRUNC, 0700);
    char buf[1];

    int contador = 0;
    int contador_ = 0;
    char *texto = "ULTIMA_IMAGEM=";
    char num[100];
    sprintf(num, "%d", ultima_imagem);
    while (1) {
        char char_atual = texto[contador];

        write(fd1, &char_atual, 1);
        if(contador >= 13){
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

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("USAGE:\n./q3 n_imagens\n");
        return EXIT_FAILURE;
    }

    printf("Meu pid: %d\n", getpid());

    // Registre AQUI seu handler para os sinais SIGINT e SIGTERM!
    struct sigaction s;
    s.sa_handler = sig_handler;
    sigemptyset(&s.sa_mask);
    s.sa_flags = 0;
    sigaction(SIGINT, &s, NULL);
    sigaction(SIGTERM, &s, NULL);

    baixa_imagens(atol(argv[1]));
    

    return 0;
}

```
---
```c
// Leia o HTML ou MD antes de iniciar este exercício!

// inclua libs aqui!
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

char *read_word(int fd);

typedef struct
{
    int id_thread;   // identificador da thread
    int *count_cpfs; // para as threads saberem onde guardar o resultado!
    int file_desc;   // descritor do arquivo
    pthread_mutex_t *file_mutex; // Mutex para sincronizar acesso ao arquivo
    pthread_mutex_t *count_mutex; // Mutex para sincronizar contagem de CPFs
} t_arg;

#define MAX_LINE_LENGTH 256

/*
 * Função que verifica se um char é numérico
 */
int isnum(char c)
{
    return c >= '0' && c <= '9';
}

/*
 * Função que verifica se uma palavra é um cpf válido
 * do ponto de vista do formado DDD.DDD.DDD-DD
 * onde D é um dígito
 *
 * ENTRADA:
 * char *txt: cep a ser validado
 *
 * SAIDA:
 *   (int) 0: o CPF é válido
 *   (int) 1: Formato inválido!
 *   (int) 2: Formato ok mas regra inválida!
 */
int validade_cpf(char *txt)
{
    int qtde = 0;
    for (qtde = 0; txt[qtde] != '\0'; qtde++)
        ;
    if (qtde != 14)
    {
        return 1;
    }
    if (!(isnum(txt[0]) && isnum(txt[1]) && isnum(txt[2]) && txt[3] == '.' &&
        isnum(txt[4]) && isnum(txt[5]) && isnum(txt[6]) && txt[7] == '.' &&
        isnum(txt[8]) && isnum(txt[9]) && isnum(txt[10]) && txt[11] == '-' &&
        isnum(txt[12]) && isnum(txt[13])))
        return 1;
    
    if (txt[0] == txt[1] && txt[0] == txt[2] && txt[0] == txt[4] && txt[0] == txt[5] &&
        txt[0] == txt[6] && txt[0] == txt[8] && txt[0] == txt[9] && txt[0] == txt[10] &&
        txt[0] == txt[12] && txt[0] == txt[13])
        return 2; //todos dígitos iguais
    
    int dig1 = ((((txt[0]-'0')*10 + (txt[1]-'0')*9 + (txt[2]-'0')*8 + (txt[4]-'0')*7 +
                  (txt[5]-'0')*6 + (txt[6]-'0')*5 + (txt[8]-'0')*4 + (txt[9]-'0')*3 +
                  (txt[10]-'0')*2)*10)%11)%10;

    int dig2 = ((((txt[0]-'0')*11 + (txt[1]-'0')*10 + (txt[2]-'0')*9 + (txt[4]-'0')*8 +
                  (txt[5]-'0')*7 + (txt[6]-'0')*6 + (txt[8]-'0')*5 + (txt[9]-'0')*4 +
                  (txt[10]-'0')*3 + (txt[12]-'0')*2)*10)%11)%10;
    
    if(dig1 == (txt[12]-'0') && dig2 == (txt[13]-'0')) return 0; //regra ok
    else return 2; //cpf inválido
}

/*
 * Faz validação de cpf em Thread.
 *
 * A thread recebe um descritor de arquivo e deve ler
 * palavras dele enquanto o arquivo não acabar!
 *
 */
void *cpf_validation_thread(void *_arg)
{
    t_arg *arg = (t_arg *)_arg;

    while (1)
    {
        char *palavra;

        pthread_mutex_lock(arg->file_mutex);
        palavra = read_word(arg->file_desc);
        pthread_mutex_unlock(arg->file_mutex);

        if (palavra == NULL)
            break;

        int ret = validade_cpf(palavra);
        if (ret == 0)
        {
            pthread_mutex_lock(arg->count_mutex);
            (*arg->count_cpfs)++;
            pthread_mutex_unlock(arg->count_mutex);
        }

        free(palavra);
    }
    return NULL;
}

/* Função que lê uma palavra de um arquivo.
 * Considere que o arquivo contém apenas uma linha, sem \n no final.
 * A linha possui diversas palavras, separadas por um único espaço
 *
 * Entradas:
 *   int fd: descritor do arquivo
 *
 * Saída:
 *   Ponteiro (char *) para uma string com a palavra lida. Caso o arquivo termine, retorne NULL.
 */
char *read_word(int fd)
{
    char buffer[1];
    char *palavra = malloc(MAX_LINE_LENGTH * sizeof(char));
    int pos = 0;
    int length = 0;

    while (1)
    {
        length = read(fd, buffer, 1);
        if (length == 0)
        {
            if (pos == 0)
            {
                free(palavra);
                return NULL; // End of file reached
            }
            else
            {
                palavra[pos] = '\0';
                return palavra;
            }
        }
        else
        {
            if (buffer[0] == ' ')
            {
                palavra[pos] = '\0';
                return palavra;
            }
            else
            {
                palavra[pos] = buffer[0];
                pos++;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("USAGE:\n./q4 <n_threads> <file_name>\n");
        return EXIT_FAILURE;
    }

    int n_threads = atoi(argv[1]); // Info da linha de comando
    char *file_name = argv[2];     // Info da linha de comando

    int fd1 = open(file_name, O_RDONLY); // Abre o arquivo

    if (fd1 == -1)
    {
        fprintf(stderr, "Falha ao abrir o arquivo!\n");
        exit(EXIT_FAILURE);
    }

    // Inicializa contador de CPFs encontrados
    int count_cpfs = 0;

    // Aloca espaço para tids e args das threads
    pthread_t *tids = malloc(sizeof(pthread_t) * n_threads);
    t_arg **args = malloc(sizeof(t_arg *) * n_threads);

    pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

    for (int i = 0; i < n_threads; i++)
    {
        t_arg *arg = malloc(sizeof(t_arg));
        arg->file_desc = fd1;
        arg->count_cpfs = &count_cpfs;
        arg->id_thread = i;
        arg->file_mutex = &file_mutex;
        arg->count_mutex = &count_mutex;
        args[i] = arg;

        pthread_create(&tids[i], NULL, cpf_validation_thread, arg);
    }

    for (int i = 0; i < n_threads; i++)
    {
        pthread_join(tids[i], NULL);
    }

    printf("RESULTADO:\nFORAM ENCONTRADOS %02d CPFS VALIDOS\n", count_cpfs);

    for (int i = 0; i < n_threads; i++)
        free(args[i]);

    close(fd1);
    free(tids);
    free(args);

    pthread_mutex_destroy(&file_mutex);
    pthread_mutex_destroy(&count_mutex);

    return 0;
}
```
