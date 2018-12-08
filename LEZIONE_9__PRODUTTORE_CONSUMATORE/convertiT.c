#include "../shared/xerrori.h"

#include <stdio.h>
#include <stdlib.h>


#define Buf_size 10
#define Max_line_len 100

int main(int argc, char* argv[]){
    if(argc != 3)
    {
        fprintf(stderr, "\nUso: %s infile outfile", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* fp = xfopen(argv[1], "r");

    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    while((read = getline(&line, &len, fp)) != EOF){

        // SCRIVI SUL BUFFER IN

    }

    xfclose(fp);
    return 0;
}