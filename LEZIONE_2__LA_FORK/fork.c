#include "../shared/xerrori.h"

int main(int argc, char const *argv[])
{
    int n = 3;
    for (int i = 0; i < n; i++)
    {
        pid_t p = xfork(__FILE__, __LINE__);
        if (p == 0)
        { //FIGLIO

            printf("Io sono %d figlio %d-esimo di %d, il principe dei Nani\n", getpid(), i, getppid());
            exit(i);
        }
    }
    printf("Io sono %d figlio di %d, il re sotto la Montagna\n", getpid(), getppid());
    for (int i = 0; i < n; i++)
    {
        int status;
        pid_t p = xwait(&status, __FILE__, __LINE__);
        printf("E' terminato il figlio %d\n", p);
        if (WIFEXITED(status) >= 0)
        {
            printf("Exit status del figlio: %d\n", WEXITSTATUS(status));
        }
    }

    printf("FINITO!\n");
    return (0);
}
