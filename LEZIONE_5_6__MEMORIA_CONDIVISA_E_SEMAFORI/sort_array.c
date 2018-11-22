#include "../shared/xerrori.h"
#include <stdlib.h>

void* create_buffer(int n, char* name, size_t dim){
  int shm_size = n*dim; // numero di byte necessari
  int fd = xshm_open(name,O_RDWR | O_CREAT, 0600);
  xftruncate(fd, shm_size);
  void *a = simple_mmap(shm_size,fd);
  close(fd); // dopo mmap e' possibile chiudere il file descriptor
  return a;
}


// genera array di n elementi con interi random tra 0 e 1999 
int *random_buffer(int n, char* name)
{
  assert(n>0);
  int *a = create_buffer(n,name, sizeof(int));
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
    int* array1 = random_buffer(n, "/array_1");
    int* array2 = create_buffer(n, "/array_2", sizeof(int));

    
    #ifdef DEBUG
    int s_ori = somma(array1, n);
    #endif

    pid_t pid = xfork();
    if(pid == 0){ //FIGLIO

        int j = n-1;
        for(int i = 0; i<n;i++){
            if(array1[i]>=1000)
                array2[j--] = array1[i];
        }
        int* start = array2+j;
        int len = n-j;
        qsort(start, len, sizeof(int), intcmp); // Ordino la prima parte del vettore

        #ifdef DEBUG
            assert(check_sort(start, len));
        #endif

        xmunmap(array1,n*sizeof(int));
        xmunmap(array2,n*sizeof(int));
        exit(EXIT_SUCCESS);
    }

    //  Ottengo nella prima parte del vettore
    //  gli elementi < 1000
     int j = 0;
    for(int i = 0; i<n;i++){
        if(array1[i]<1000)
            array2[j++] = array1[i];
    }
    int* start = array2;
    int len = j;
    qsort(start, len, sizeof(int), intcmp); // Ordino la prima parte del vettore
    #ifdef DEBUG
        assert(check_sort(array2, j));
    #endif

    xwait(NULL);

    #ifdef DEBUG
    int s_aft = somma(array2, n);
    assert(s_aft==s_ori);
    assert(check_sort(array2, n));
    #endif

    xmunmap(array1,n*sizeof(int));
    xmunmap(array2,n*sizeof(int));
    xshm_unlink("/array_1");
    xshm_unlink("/array_2");
    return 0;
}