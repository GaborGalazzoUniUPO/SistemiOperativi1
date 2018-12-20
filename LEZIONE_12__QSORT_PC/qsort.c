#define _GNU_SOURCE
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include "xerrors.h"

#define DIM_BUFFER 10

// genera array di n elementi con interi random tra 0 e 999999
int *random_array(int n, int seed)
{
    assert(n > 0);
    srandom(seed);
    int *a = malloc(n * sizeof(int));
    assert(a != NULL);
    for (int i = 0; i < n; i++)
        a[i] = (int)random() % 1000000;
    return a;
}

// crea una copia dell'array a
int *copy_array(int *a, int n)
{
    int *b = (int *)malloc(n * sizeof(int));
    assert(b != NULL);
    for (int i = 0; i < n; i++)
        b[i] = a[i];
    return b;
}

// verifica che l'array a[0..n] sia uguale a b[0..n]
bool check_sort(int *a, int *b, int n)
{
    for (int i = 0; i < n; i++)
        if (a[i] != b[i])
            return false;
    return true;
}

// funzione di confronto tra interi passata a qsort
int intcmp(const void *a, const void *b)
{
    return *((int *)a) - *((int *)b);
}

// prototipo quicksort e partition
void quicksort(int a[], int, int);
int partition(int A[], int p, int r);

typedef struct
{
    int p;
    int r;
} couple_t;


typedef struct
{

    couple_t* buf;
    sem_t* sem_free_slots;
    sem_t* sem_data_items;
    int* array;
    int* cindex;
    pthread_mutex_t* mutex;

} cargs_t;

void* cbody(void *args)
{
    cargs_t *cargs = (cargs_t *)args;
    couple_t couple;
    while (true)
    {
        xsem_wait(cargs->sem_data_items,__LINE__,__FILE__);
        xpthread_mutex_lock(cargs->mutex,__LINE__,__FILE__);
        couple = cargs->buf[*(cargs->cindex) % DIM_BUFFER];
        *(cargs->cindex) += 1;
        xpthread_mutex_unlock(cargs->mutex,__LINE__,__FILE__);
        xsem_post(cargs->sem_free_slots,__LINE__,__FILE__);
        if(couple.p > couple.r)
            return NULL;
        quicksort(cargs->array, couple.p, couple.r);
    }
    return NULL;
}

void very_quicksort(int *A, int p, int r, int limite, int* pindex, couple_t* couples, 
sem_t *sem_free_slots, sem_t *sem_data_items)
{
    if (p < r)
    {
        if (r - p <= limite)
        {
            xsem_wait(sem_free_slots, __LINE__,__FILE__);
            couple_t c;
            c.p = p;
            c.r = r;
            couples[*pindex % DIM_BUFFER] = c;
            (*pindex)++;
            xsem_post(sem_data_items, __LINE__,__FILE__);
        }
        else
        {
            // esegui come nel caso senza thread:
            int q = partition(A, p, r);
            very_quicksort(A, p, q, limite, pindex, couples, sem_free_slots, sem_data_items);
            very_quicksort(A, q + 1, r, limite, pindex, couples, sem_free_slots, sem_data_items);
        }
    }
}

// Quicksort: ordina A[p]...A[r] (p<=r)
// il numero di elementi totale da ordinare e' 1+r-p
void quicksort(int *A, int p, int r)
{
    int q;

    if (p < r)
    { // se p==r  c'e'un solo elemento e quindi non si deve ordinare
        q = partition(A, p, r);
        quicksort(A, p, q);
        quicksort(A, q + 1, r);
    }
}

/* ***********************************************************
   partition con scelta pivot mediano di 3 
   per evitare l'uso di random che e' lenta e non thread safe
   *********************************************************** */
int partition(int A[], int p, int r)
{
    int t, x;
    int i, j;

    // il pivot sara' il mediano tra A[p],A[i],A[r]
    i = (p + r) / 2;
    if (A[p] > A[i])
    {
        t = A[i];
        A[i] = A[p];
        A[p] = t;
    } // scambia A[i]<->A[p]
    if (A[p] > A[r])
    {
        t = A[r];
        A[r] = A[p];
        A[p] = t;
    } // scambia A[r]<->A[p], ora A[p] e' il minino
    if (A[i] > A[r])
    {
        t = A[r];
        A[r] = A[i];
        A[i] = t;
    } // scambia A[r]<->A[i], ora A[p]<= A[i] <= A[r]

    x = A[i]; // pivot
    t = A[i];
    A[i] = A[p];
    A[p] = t; // scambia A[i]<->A[p]

    i = p - 1;
    j = r + 1;
    while (1)
    {
        while (A[--j] > x)
            ; // scan da destra a sinistra: esce se A[j]<=x
        while (A[++i] < x)
            ; // scan da sinistra a destra: esce se A[i]>=x
        if (i < j)
        {
            t = A[i];
            A[i] = A[j];
            A[j] = t; // scambia A[i] <-> A[j] e riprendi scansioni
        }
        else
            break; // se i>=j termina
    }
    return j;
}



int main(int argc, char *argv[])
{
    int n, numth, limite, seed;

    if ((argc < 4) || (argc > 5))
    {
        fprintf(stderr, "USO:  %s numel numth limite [seed]\n\n", argv[0]);
        exit(1);
    }
    n = atoi(argv[1]);
    if (n < 1)
    {
        fprintf(stderr, "Input non valido (main)\n");
        exit(1);
    }
    numth = atoi(argv[2]);
    if (numth < 1)
    {
        fprintf(stderr, "Input non valido (main)\n");
        exit(1);
    }

    limite = atoi(argv[3]);
    if (limite < 1)
    {
        fprintf(stderr, "Input non valido (main)\n");
        exit(1);
    }

    /* ----------- legge il seme per i numeri casuali ----------- */
    if (argc == 3)
        seed = atol(argv[4]);
    else
        seed = 3141562;

    // genera array random
    int *a = random_array(n, seed);
    // copia array e ordina la copia usando la funzione di libreria
    int *copia = copy_array(a, n);
    qsort(copia, n, sizeof(int), intcmp);

    //CREO IL BUFFER
    couple_t couples[DIM_BUFFER];
    int pindex = 0;
    int cindex = 0;
    sem_t sem_free_slots, sem_data_items;
    // inizializzazione semafori
    xsem_init(&sem_free_slots, 0, DIM_BUFFER,__LINE__,__FILE__);
    xsem_init(&sem_data_items, 0, 0,__LINE__,__FILE__);

   

    pthread_mutex_t mutex;
    xpthread_mutex_init(&mutex,NULL,__LINE__,__FILE__);

    cargs_t args;
    args.array = a;
    args.buf = couples;
    args.sem_data_items = &sem_data_items;
    args.sem_free_slots = &sem_free_slots;
    args.cindex = &cindex;
    args.mutex = &mutex;
    
    // CREO I CONSUMATORI
    pthread_t threads[numth];
    for (int i = 0; i < numth; i++)
    {
        xpthread_create(threads+i, NULL, cbody, &args,__LINE__,__FILE__);
    }

    // eseguo il mio algoritmo di ordinamento
    very_quicksort(a, 0, n - 1, limite, &pindex, couples, &sem_free_slots, &sem_data_items);

    for (int i = 0; i < numth; i++)
    {
        xsem_wait(&sem_free_slots, __LINE__,__FILE__);
        couple_t c;
        c.p = 1;
        c.r = 0;
        couples[pindex%DIM_BUFFER] = c;
        pindex++;
        xsem_post(&sem_data_items, __LINE__,__FILE__);
    }

    for (int i = 0; i < numth; i++)
    {
        xpthread_join(threads[i],NULL, __LINE__,__FILE__);

    }

    // confronto a[] ordinato da me con copia[]
    if (check_sort(a, copia, n))
        fprintf(stderr, "Sort OK\n");
    else
        fprintf(stderr, "Sorting fallito\n");
    return 0;
}