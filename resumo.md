# Aula 10 - Processos

### Sistemas Operacionais
- **Kernel**: Software do sistema que gerencia programas, memória e recursos do hardware.
- **Processo do usuário**: Qualquer programa sendo executado no computador (a falha de umprocesso não afeta os outros) e realiza chamadas ao kernel.

#### POSIX (Portable Operating System Interface): family of standarts specified by the IEEE for maintaining compatibility between operating systems.

---

### Processos e Fluxo de Controle
- **Fluxo de controle**: sequência de instruções lida e executada pela CPU. Pode ser alterado por saltos, desvios, chamadas e retornos para responder a eventos no sistema.
- **Interrupções**: permitem que o sistema operacional reaja a eventos externos (ex.: entrada de dados, erros de execução, sinais de teclado).

---

### Multiprocessamento
- **Ilusão de multiprocessamento**: cada programa aparenta uso exclusivo da CPU e memória.
- **Realidade**: o sistema operacional intercala a execução de processos, salvando e restaurando registradores e endereços conforme necessário (chaveamento de contexto).

---

### Criação de Processos e Relação Pai e Filho
- **Criação de processos**: Um processo é criado através da chamada de sistema `fork`, que cria um novo processo filho como uma cópia do processo pai.

  - No processo pai, `fork` retorna o PID do filho, permitindo que o pai  identifique e acompanhe o filho.
  - No processo filho, `fork` retorna 0, indicando que o processo é o filho.

- **Funções associadas**:
  - **getpid()**: retorna o PID do processo atual (pai ou filho).
  - **getppid()**: no processo filho, retorna o PID do processo pai.

---

### As chamadas wait/waitpid
Um processo pode esperar seus filhos acabarem usando uma das chamadas `pid_t wait(int *wstatus)` ou `pid_t waitpid(pid_t pid, int *wstatus, int options)`. A primeira espera qualquer um dos filhos, enquanto a segunda espera um filho (ou grupo de filhos) específico. Ambas retornam o PID do filho que terminou. 

OBS: O valor de retorno do processo é retornado via o ponteiro `wstatus`

---

# Aula 11 - Carregamento de Programas

### A chamada `execvp`

`int execvp(const char *file, char *const argv[]);`


A chamada `execvp` faz duas coisas:
1. Carrega um programa na memória dentro do contexto do processo atual
2. Inicia esse programa, preenchendo os argumentos do `main`

O programa que estava em execução antes do execvp é completamente
destruído.

---

# Aula 12 - Entrada/Saída

As chamadas `open`, `close`, `read` e `write`.

---

### Syscalls para arquivos (regulares)
- Abrir e fechar arquivos: **open()** e **close()**
- Ler e escrever em arquivos: **read()** e **write()**
- Mudar posição corrente no arquivo: **lseek()**

---

### Visão Geral das Syscalls de Arquivos
As chamadas de sistema POSIX para manipulação de arquivos incluem:

### Abertura e fechamento de arquivos:

- ``open()``: Abre um arquivo e retorna um file descriptor (descritor de arquivo), um número inteiro usado para referenciar o arquivo.
  - Parâmetro **flags**: Define as permissões e comportamentos da abertura. Ex.: ``O_RDONLY`` (somente leitura), ``O_WRONLY`` (somente escrita), ``O_RDWR`` (leitura e escrita), ``O_CREAT`` (cria o arquivo se ele não existir).
  - Parâmetro **mode**: Especifica as permissões do arquivo criado.

- ``close()``: Fecha o arquivo referenciado pelo file descriptor, informando ao kernel que o arquivo não será mais acessado.

### Leitura e Escrita de Arquivos:

- ``read(int fd, void *buf, size_t count)``: Lê até **count** bytes do arquivo referenciado por **fd** e armazena em **buf**.

- ``write(int fd, const void *buf, size_t count)``: Escreve até **count** bytes do **buf** no arquivo **fd**.

  - Ambas retornam o número de bytes lidos/escritos e -1 em caso de erro.
  - Short counts: Em certas condições (ex.: ao ler linhas do terminal ou sockets), as funções ``read`` e ``write`` podem retornar menos bytes do que o solicitado.

### Mover a Posição Corrente no Arquivo:

- ``lseek()``: Modifica a posição de leitura/escrita no arquivo, útil para manipulação avançada de arquivos.

---

### Arquivos como Representação de Dispositivos em Unix
- No Unix, **todos os dispositivos de entrada/saída são representados como arquivos**, inclusive dispositivos como /dev e estruturas de sistema como /proc e /sys, permitindo que usuários interajam com hardware e processos através do sistema de arquivos.

---

### E/S Padrão
Todo processo criado por um shell Linux possui três arquivos já abertos, conhecidos como:
- 0: stdin (entrada padrão)
- 1: stdout (saída padrão)
- 2: stderr (erro padrão)

---

# Aula 13 - Enviando sinais

### (Alguns) Sinais POSIX:
![alt text](imgs/(Alguns)SinaisPOSIX.png)

### Conceito de Sinais POSIX
**Sinais POSIX** são notificações assíncronas enviadas para processos para indicar a ocorrência de eventos específicos. São usados principalmente para informar que:
- Ocorreu um erro ou exceção de hardware.
- Houve uma alteração na condição do sistema.
- O usuário deseja interromper ou finalizar um processo.

---

### Envio de Sinais
Um sinal pode ser enviado de duas formas principais:
- **Kernel**: Detecta um evento no sistema, como uma divisão por zero (gera **SIGFPE**) ou o término de um processo filho (gera **SIGCHLD**).
- **Outro processo**: Pode enviar sinais explicitamente usando a chamada de sistema **kill**, solicitando ao kernel que envie o sinal para o processo alvo.

---

### Exemplo de Chamada de Sistema ``kill``
- **kill(pid, sig)**: A chamada de sistema ``kill`` permite que um processo envie um sinal para outro processo, identificado pelo ``pid``. O sinal enviado é especificado por ``sig``, podendo ser qualquer sinal POSIX.

---

# Aula 14 - Capturando Sinais

### Revisão de Sinais
- **wait** e **kill**: Chamadas de sistema usadas para gerenciar e enviar sinais a processos.

---

### Recebendo Sinais
O kernel força o processo a responder ao sinal recebido. O processo pode:
- **Ignorar** o sinal.
- **Terminar** o processo (opcionalmente com core dump).
- **Capturar** o sinal, executando um signal handler personalizado.

Exemplo prático de uso:
- Confirmar saída do programa ou adicionar limite de tempo.

---

### Função sigaction
**Declaração**: ``int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);``

A função permite definir como um processo reage a sinais específicos.

Estrutura ``sigaction`` inclui:
- ``sa_handler``: Função para tratar o sinal.
- ``sa_mask``: Sinais a serem bloqueados durante o handler.
- ``sa_flags``: Opções de configuração.

---

### Concorrência e Sinais
Problemas surgem quando múltiplos handlers tentam modificar a mesma variável ou acessar recursos globais (como ``errno``).

**Handlers Aninhados**: Um handler pode ser interrompido por outro handler de sinal diferente, criando um fluxo complexo que requer atenção para evitar conflitos.

---

### Bloqueio Temporário de Sinais
É possível "bloquear" sinais durante a execução de um handler. Isso mantém o sinal pendente até ser desbloqueado.

---

# Aula 15 - Concorrência e Threads

### Processos X Threads
**Processos**
- Execução paralela ou concorrente
- **Espaços de endereçamento separados**
- Compartilham algumas estruturas (tabela de descritores de arquivo, etc)
- Possível distribuir em várias máquinas

**Threads** : serve para dividir um programa em unidades de execução independentes, permitindo que várias tarefas ocorram simultaneamente dentro do mesmo processo.
- Executam no mesmo processo e sempre pertencem a um único processo
- Mesmo espaço de endereçamento
  - **Compartilham memória**
- **Não controlamos a execução de threads**, o sistema operacional faz isso
- Sincronização para acessar recursos
compartilhados

---

# Aula 16 - Sincronização com Mutex

### Mutex
**Mutex** (Mutual Exclusion): Permite sincronizar o acesso de threads a recursos compartilhados, garantindo que apenas uma thread acesse a região crítica por vez.

---

### Operações Principais com Mutex
1. **Inicialização**:
   - `pthread_mutex_t mutex;` — Declaração de uma variável mutex.
   - `pthread_mutex_init(&mutex, NULL);` — Inicializa o mutex com valores padrão.

2. **Bloqueio e Desbloqueio**:
   - `pthread_mutex_lock(&mutex);` — Bloqueia o mutex, impedindo acesso de outras threads à região crítica.
   - `pthread_mutex_unlock(&mutex);` — Desbloqueia o mutex, liberando o acesso.

3. **Destruição**:
   - `pthread_mutex_destroy(&mutex);` — Libera recursos alocados pelo mutex após o uso.

---

# Aula 17 - Semáforos

Semáforos são uma forma de sincronização, usado para que threads sincronizem seu progresso e possam executar em paralelo.

Duas tarefas podem ser feitas em paralelo se:
- elas **não compartilham absolutamente nenhuma informação**.
- elas **compartilham informação mas possuem mecanismos de sincronização** de tal maneira que toda ordem de execução possível de suas instruções resulte no mesmo resultado final.