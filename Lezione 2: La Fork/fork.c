#include "xerrori.h"

int main(int argc, char const *argv[])
{
    pid_t p = xfork(__FILE__, __LINE__);
    if(p == 0){ //FIGLIO

        printf("Io sono %d figlio di %d, il principe dei Nani\n", getpid(), getppid());
    }else{ //PADRE
        printf("Io sono %d padre di %d, il re sotto la Montagna\n", getpid(), p);
    }
    printf("FINITO!\n");
}
