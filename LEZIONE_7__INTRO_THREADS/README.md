Lezione 7: introduzione ai threads
----------------------------------

Argomenti trattati:

-   Abbiamo ridiscusso il meccanismo usato per creare una zona di memoria condivisa tra più processi. In pratica si tratta di un file che risiede in RAM e mappato in un array attraverso *mmap*. Abbiamo visto un esempio di uso di *mmap* per mappare un file residente su disco su un array e osservato che questo rende molto più semplice lavorare su un file.

-   Abbiamo visto l'istruzione *pthread_create* per creare un nuovo thread e discusso il meccanismo con cui possono essere passati al thread i dati su cui lavorare e con cui i thread possono passare risultati al thread principale

-   Abbiamo visto alcuni esempi di calcolo di primi in parallelo utilizzando più thread. L'ultimo degli esempi (tabellaprimi_threads.c) non è completo: per casa aggiungere il semaforo necessario per regolare l'accesso alla tabella condivisa.

-   Il semafori per i thread si creano e si utilizzano come per i processi: l'unica differenza è che il secondo argomento della *sem_init* è zero in quanto il semaforo non deve essere condiviso tra più processi.

### Esercizio per casa

Scrivere una funzione `int massimo(int a[], int n, int p)` che dato un array *a[ ]* di *n* interi restituisce il massimo elemento dell'array utilizzando *p* thread. Siete liberi di organizzare il calcolo come preferite ma ogni thread deve leggere al massimo *n/p+1* elementi dell'array.

Per testare la vostra soluzione potete usare il codice qui sotto che costruisce un array random e calcola il suo massimo in maniera tradizionale.

```c

#define _GNU_SOURCE
#include < stdio.h>
#include < stdlib.h>
#include < sys/types.h>
#include < sys/wait.h>
#include < unistd.h>
#include < stdbool.h>
#include < assert.h>

// prototipi di funzione
int *random_array(int n);
int massimo(int *a, int n, int p);

// main
int main(int argc, char **argv)
{
  if(argc!= 3)
  {
    fprintf(stderr,"Uso:\n\t %s n p\n",argv[0]);
    exit(1);
  }
  int n= atoi(argv[1]);
  assert(n > 0);
  int p = atoi(argv[2]);
  assert(p > 0);

  int *a = random_array( n );
  int max = a[0];
  for(int i=1; i < n ;i++)
    if(a[i]>max) max=a[i];

  printf("Massimo calcolato senza thread ausiliari: %d\n",max);

  // questa e' la funzione da scrivere per esercizio
  max = massimo(a,n,p);
  printf("Massimo calcolato con %d thread: %d\n",p, max);

  free(a);
  return 0;
}

// calcola il massimo dell'array a[] di n elementi
// utilizzando p thread
// ogni thread deve leggere al piu' (n/p)+1 elementi
int massimo(int *a, int n, int p) {
    return -1;
}

// genera array di n elementi con interi random tra -100000 e 100000
int *random_array(int n)
{
  assert(n>0);
  int *a = malloc(n* sizeof(int));
  assert(a!=NULL);
  for(int i=0; i < n ;i++)
    a[i] = (random()%200001) - 100000;
  return a;
}

```

Ultime modifiche: giovedì, 29 novembre 2018, 21:19