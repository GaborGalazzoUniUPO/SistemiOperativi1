#include "xerrori.h"

int main(int argc, char const *argv[])
{
    for (int i = 0; i < 100; i++)
    {
        pid_t p = xfork(__FILE__, __LINE__);
        if (p == 0)
        { //FIGLIO

            printf("Io sono %d figlio %d-esimo di %d, il principe dei Nani\n", getpid(), i, getppid());
            exit(0);
        }
    }
    printf("Io sono %d figlio di %d, il re sotto la Montagna\n", getpid(), getppid());
    printf("FINITO!\n");
    return (0);
}
