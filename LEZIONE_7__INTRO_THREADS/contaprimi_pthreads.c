#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <pthread.h>

typedef struct
{
    int *a;
    int size;
    int max;
} targs_t;

// prototipi di funzione
int *random_array(int n);
int massimo(int *a, int n, int p);

// main
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Uso:\n\t %s n p\n", argv[0]);
        exit(1);
    }
    int n = atoi(argv[1]);
    assert(n > 0);
    int p = atoi(argv[2]);
    assert(p > 0);

    int *a = random_array(n);
    int max = a[0];
    for (int i = 1; i < n; i++)
        if (a[i] > max)
            max = a[i];

    printf("Massimo calcolato senza thread ausiliari: %d\n", max);

    // questa e' la funzione da scrivere per esercizio
    max = massimo(a, n, p);
    printf("Massimo calcolato con %d thread: %d\n", p, max);

    free(a);
    return 0;
}

void *
tbody(void *args)
{
    targs_t *targs = (targs_t *)args;
    targs->max = targs->a[0];
    for (int i = 1; i < targs->size; i++)
        if (targs->a[i] > targs->max)
            targs->max = targs->a[i];
    printf("%d\n", targs->max);
    pthread_exit(NULL); 
}

// calcola il massimo dell'array a[] di n elementi
// utilizzando p thread
// ogni thread deve leggere al piu' (n/p)+1 elementi
int massimo(int *a, int n, int p)
{
    pthread_t threads[p];
    targs_t targs[p];
    for (int i = 0; i < p; i++)
    {
        targs[i].a = a + (i * (n / p));
        targs[i].size = n / p;

        int e = pthread_create(threads + i, NULL, tbody, targs);
        if (e < 0)
        {
            perror("ERRORE PTHREAD CREATE");
            exit(1);
        }
    }
    int max = a[0];
    for (int i = 0; i < p; i++)
    {
        int e = pthread_join(threads[p], NULL);
        if (e < 0)
        {
            perror("ERRORE PTHREAD JOIN");
            exit(1);
        }
        if (targs[p].max > max)
        {
            max = targs[p].max ;
        }
    }
    return max;
}

// genera array di n elementi con interi random tra -100000 e 100000
int *random_array(int n)
{
    srand(time(NULL));
    assert(n > 0);
    int *a = malloc(n * sizeof(int));
    assert(a != NULL);
    for (int i = 0; i < n; i++)
        a[i] = (random() % 200001) - 100000;
    return a;
}
