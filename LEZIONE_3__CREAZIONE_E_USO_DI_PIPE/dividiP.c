#include "../shared/xerrori.h"
#include <string.h>

// Somma pari: 2250 somma dispari: -679

void perform_action(int pipe[2], const String filename, const int usless_pipe[2]){
    xclose(pipe[1]); //NON SCRIVO
    xclose(usless_pipe[0]);
    xclose(usless_pipe[1]);
    FILE* out = fopen(filename, "wt");
    int acc = 0;
    while(true) {
        int x;
        ssize_t e = xread(pipe[0],&x,sizeof(int));
        if(e==0) break;
        acc += x;
        fprintf(out, "%d\n", x);

        
    } 
    printf("Somma dei numeri %s letti da %d: %d\n",filename, getpid(), acc);

    //FINITO PROCESSO
    xfclose(out);
    xclose(pipe[0]); 
    exit(EXIT_SUCCESS);
}

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
        perform_action(down_odd, strcat(argv[1],".pari"), down_even);
    }

    pid_t p_eve = xfork();
    if(p_eve == 0){ //EVE child
        perform_action(down_even, strcat(argv[1],".dispari"), down_odd);
    }

    xclose(down_odd[0]); //NON LEGGO
    xclose(down_even[0]); //NON LEGGO

    int n;
    //int i = 0;
    while(fscanf(fp, "%d", &n) != EOF){
        xwrite(n%2?down_even[1]:down_odd[1], &n, sizeof(int)); 
        //xwrite(down_odd[1], &n, sizeof(int));
    }
    xclose(down_odd[1]); //FINITO
    xclose(down_even[1]);
    xfclose(fp);
    exit(EXIT_SUCCESS);
}