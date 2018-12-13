#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../shared/xerrori.h"

void* pbody(void* args){
    char* infile = (char*) args;
    

}

int main(int argc, char* argv[]){

    if(argc < 4){
        fprintf(stderr, "USO: %s infile1 infile2 infile3 ... infileN outfile numt", argv[0]);
        exit(EXIT_FAILURE);
    }

    int numt = atoi(argv[argc-1]);
    assert(numt > 0);

    char* outfile = argv[argc-2];
    char* infile = argv[argc-3];

    // CREAO N THREAD PRODUTTORI
    pthread_t tproducer[argc-4];
    for(int i = 0; i<argc-4; i++){
        xpthread_create(tproducer[i+1], NULL, pbody, argv[i+1]);

    }

}