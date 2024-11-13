#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t p2, p3, p4, p5;

    printf("Processo P1 (Pai): PID=%d\n", getpid());

    // Cria o processo P2
    p2 = fork();
    if (p2 == 0) { // Código do processo P2
        printf("Processo P2: PID=%d, PID do pai=%d\n", getpid(), getppid());
        return 0; // P2 termina
    } else if (p2 > 0) {
        // Cria o processo P3 no processo pai P1
        p3 = fork();
        if (p3 == 0) { // Código do processo P3
            printf("Processo P3: PID=%d, PID do pai=%d\n", getpid(), getppid());

            // Cria o processo P4 dentro do processo P3
            p4 = fork();
            if (p4 == 0) { // Código do processo P4
                printf("Processo P4: PID=%d, PID do pai=%d\n", getpid(), getppid());
                return 0; // P4 termina
            }

            // Cria o processo P5 dentro do processo P3
            p5 = fork();
            if (p5 == 0) { // Código do processo P5
                printf("Processo P5: PID=%d, PID do pai=%d\n", getpid(), getppid());
                return 0; // P5 termina
            }

            // Processo P3 espera que P4 e P5 terminem
            wait(NULL);
            wait(NULL);
            return 0; // P3 termina
        }
    }

    // Processo pai P1 espera que P2 e P3 terminem
    wait(NULL);
    wait(NULL);

    return 0;
}
