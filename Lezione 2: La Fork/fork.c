#include "xerrori.h"

int main(int argc, char const *argv[])
{
    for (int i = 0; i < 2; i++)
    {
        pid_t p = xfork(__FILE__, __LINE__);
        if (p == 0)
        { //FIGLIO

            printf("Io sono %d figlio %d-esimo di %d, il principe dei Nani\n", getpid(), i, getppid());
            exit(0);
        }
    }
    printf("Io sono %d padre di %d e figlio di %d, il re sotto la Montagna\n", getpid(), p, getppid());
    printf("FINITO!\n");
    return (0);
}
