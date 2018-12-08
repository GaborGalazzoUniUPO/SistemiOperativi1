#include "../shared/xerrori.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>


// genera array di n elementi con interi random tra 0 e 1999
int *random_array(int n)
{
  assert(n>0);
  int* a = malloc(n* sizeof(int));
  if(a==NULL) die("errore allocazione");
  for(int i=0;i < n;i++)
    a[i] = (int) random() % 2000;
  return a;
}

// verifica che array a[0..n-1] sia ordinato 
bool check_sort(int a[], int n)
{
  for(int i=0; i < n-1; i++)
     if(a[i] > a[i+1]) return false;
  return true;
}

// calcola somma elementi array
int somma_array(int a[], int n)
{
  int somma=0;
  for(int i=0;i < n;i++)
    somma += a[i];
  return somma;
}

// funzione di confronto tra gli interi puntati da a e b
// da usare con la funzione di libreria qsort 
int intcmp(const void *a, const void *b)
{
  return *((int *) a) - *((int *) b);
}

typedef struct {
    /*
    int* s_array;
    int* t_array;
    int  ta_size;
    int limite;
    */
    int n;
    int limite;
    int* a;
    int* b;
} targs_t;

void mt_mergesort(int a[], int n, int limite);

void* tbody(void* gargs){

    targs_t* args = (targs_t*) gargs;
    for(int i = 0; i<args->n; i++){
        args->b[i] = args->a[i];
    }
    mt_mergesort(args->b, args->n, args->limite);

    return NULL;
}

void mt_mergesort(int a[], int n, int limite)
{
   if(n<=limite) {
        // caso base usa la funzione di libreria qsort
        qsort(a,n,sizeof(int),intcmp);
   }
    else {

        // alloca due array b e c di lunghezza n/2 e n-n/2 
        int* b = malloc(sizeof(int)*(n/2));
        int* c = malloc(sizeof(int)*(n-(n/2)));

        // in parallelo:
        pthread_t* pthread = malloc(sizeof(pthread_t));
        //   copia i primi n/2 elementi di a in b
        //   invoca mt_mergesort(b,n/2,limite)
        targs_t* targs = malloc(sizeof(targs_t));
        targs->a = a;
        targs->b = b;
        targs->n = n/2;
        targs->limite = limite;
        xpthread_create(pthread, NULL, tbody, targs);


        //copia i secondi  n-n/2 elementi di a in c
        //invoca mt_mergesort(c,n-n/2,limite)
        for(int i = 0; i<(n-(n/2)); i++){
            c[i] = a[(n/2)+i];
        }
        mt_mergesort(c, n-(n/2), limite);

        // quando entrambi i thread hanno finito   
        xpthread_join(*pthread, NULL);

        // esegui il merge di b[] e c[] in a[]
        for(int i = 0, j = 0, k = 0; i<n; i++){
            if( j!=(n/2) && (b[j]<c[k] || k == (n-(n/2))))
                a[i] = b[j++];
            else
                a[i] = c[k++];
        }
        // dealloca b[] e c[]
        free(b);
        free(c);
        free(pthread);
        free(targs);

    }
} 


int main(int argc, char *argv[])
{
  int n, limite;

  if((argc<2)||(argc>3)) {
    fprintf(stderr,"USO:  %s numel limite\n\n",argv[0]);
    exit(1);
  }
  n = atoi(argv[1]);
  limite = atoi(argv[2]);
  if(n<1 || limite <1) {
    fprintf(stderr,"Input non valido (main)\n");
    exit(1);
  }
  int* a = random_array( n );
  fprintf(stderr, "Somma elementi input: %d\n", somma_array(a,n));
  mt_mergesort(a,n,limite);
  if(check_sort(a,n))
    fprintf(stderr,"Sort OK\n");
  else
    fprintf(stderr,"Sorting fallito\n");
  fprintf(stderr, "Somma elementi output: %d\n", somma_array(a,n));
  free(a);
  return 0;
}
