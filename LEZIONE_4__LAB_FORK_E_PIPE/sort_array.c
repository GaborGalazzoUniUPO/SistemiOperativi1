#include "../shared/xerrori.h"
#include <stdlib.h>


// genera array di n elementi con interi random tra 0 e 1999 
int *random_array(int n)
{
  assert(n>0);
  int *a = malloc(n* sizeof(int));
  assert(a!=NULL);
  for(int i=0;i < n;i++)
    a[i] = (int) random() % 2000;
  return a;
}

// funzione di confronto tra gli interi puntati da a e b 
int intcmp(const void *a, const void *b)
{
  return *((int *) a) - *((int *) b);
}

// verifica che array a[0..n] sia ordinato 
bool check_sort(int *a, int n)
{
  for(int i=0; i < n-1; i++)
     if(a[i] > a[i+1]) return false;
  return true;
}



#define DEBUG

#ifdef DEBUG
   #include <assert.h>
   int somma(int* array, int n){
       int somma = 0;
       for(int i = 0; i<n; i++)
       somma+=array[i];
       return somma;
   }
#endif

int main(int argc, String argv[]){
    if(argc != 2){
        printf("Uso: %s num\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    // Il processo padre legge il numero di elementi n 
    // dell'array dalla linea di comando e costruisce un 
    // array di n elementi random compresi tra 0 e 1999. 
    int n = atoi(argv[1]);
    int* array = random_array(n);

    int pipe_fd[2];
    xpipe(pipe_fd);
    
    #ifdef DEBUG
    int s_ori = somma(array, n);
    #endif

    pid_t pid = xfork();
    if(pid == 0){ //FIGLIO
        xclose(pipe_fd[0]); //Non Leggo

        //  Ottengo nella prima parte del vettore
        //  gli elementi >= 1000
        int j = 0;
        for(int i = 0; i<n; i++){
            if(array[i]>=1000){
                array[j++] = array[i];
            }
        }
        qsort(array, j, sizeof(int), intcmp); // Ordino la prima parte del vettore

        #ifdef DEBUG
            assert(check_sort(array, j));
        #endif
        xwrite(pipe_fd[1], array, j*sizeof(int));

        free(array);
        xclose(pipe_fd[1]); //Ho finito
        exit(EXIT_SUCCESS);
    }
    xclose(pipe_fd[1]); //Non Scrivo

    //  Ottengo nella prima parte del vettore
    //  gli elementi < 1000
    int j = 0;
    for(int i = 0; i<n; i++){
         if(array[i]<1000){
             array[j++] = array[i];
        }
    }
    qsort(array, j, sizeof(int), intcmp); // Ordino la prima parte del vettore
    #ifdef DEBUG
        assert(check_sort(array, j));
    #endif
    xread(pipe_fd[0], array+j, (n-j)*sizeof(int));

    #ifdef DEBUG
    int s_aft = somma(array, n);
    assert(s_aft==s_ori);
    assert(check_sort(array, n));
    #endif

    free(array);
    xclose(pipe_fd[0]); //Ho finito
    return 0;
}