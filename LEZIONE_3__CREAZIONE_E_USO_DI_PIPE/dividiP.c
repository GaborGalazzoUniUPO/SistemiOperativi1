#include "../shared/xerrori.h"

int main(int argc, String argv[]){
    if(argc!=2)
    {
        printf("Uso: %s nomefile\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* fp = xfopen(argv[1], "rt");

    int down_odd[2];
    int down_even[2];

    xpipe(down_odd);
    xpipe(down_even);

    pid_t p_odd = xfork();
    if(p_odd == 0){ //ODD child
        xclose(down_odd[0]); //NON LEGGO

        xclose(down_odd[1]); //FINITO PROCESSO
        exit(EXIT_SUCCESS);
    }

    pid_t p_eve = xfork();
    if(p_eve == 0){ //EVE child
        xclose(down_even[0]); //NON LEGGO

        xclose(down_even[1]); //FINITO PROCESSO
        exit(EXIT_SUCCESS);
    }

    xclose(down_odd[1]); //NON SCRIVO
    xclose(down_even[1]); //NON SCRIVO


    xclose(down_odd[0]); //FINITO USO PIPE
    xclose(down_even[0]);
    xfclose(fp);
}