#include "xerrori.h"

int main(int argc, char const *argv[])
{
    int n = 3;
    for (int i = 0; i < n; i++)
    {
        pid_t p = xfork(__FILE__, __LINE__);
        if (p == 0)
        { //FIGLIO

            printf("Io sono %d figlio %d-esimo di %d, il principe dei Nani\n", getpid(), i, getppid());
            exit(0);
        }
    }
    printf("Io sono %d figlio di %d, il re sotto la Montagna\n", getpid(), getppid());
    for (int i = 0; i < n; i++)
    {
        pid_t p = xwait(NULL, __FILE__, __LINE__);
        if (p == 0)
            printf("E' terminato il figlio %d", p);
    }

    printf("FINITO!\n");
    return (0);
}
