#include "xerrors.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct {
  int bytesLeft;            // bytes attualmente disponibili
  pthread_cond_t cond;      // condition variable
  pthread_mutex_t mutex;    // mutex associato alla condition variable
} heap_t;

static heap_t* heap;

void heap_init(heap_t *h, int maxSize){
    h->bytesLeft = maxSize;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    h->cond = cond;
    h->mutex = mutex;
}

int heap_alloc(heap_t *h, int size){
    printf("%ld sta cercando di ottenere il mutex per allocare\n", pthread_self());
    pthread_mutex_lock(&(h->mutex));
    printf("%ld ha ottenuto il mutex per allocare\n", pthread_self());
    while (h->bytesLeft < size)
    {
        printf("%ld aspetta ce ci sia lo spazio\n", pthread_self());
        pthread_cond_wait(&(h->cond), &(h->mutex));  
    }
    printf("%ld alloca %d\n", pthread_self(), size);
    h->bytesLeft -= size;   
    printf("%ld rilascia il mutex per allocare\n", pthread_self());            
    pthread_mutex_unlock(&(h->mutex));
    
    return 1;
}

void heap_dealloc(heap_t *h, int size) {
  printf("%ld sta cercando di ottenere il mutex per deallocare\n", pthread_self());
  pthread_mutex_lock(&(h->mutex));
  printf("%ld ha ottenuto il mutex per deallocare\n", pthread_self());
  h->bytesLeft += size;    
  printf("%ld ha restituito %d bytes e sveglia tutti\n", pthread_self(), size);  
  pthread_cond_broadcast(&(h->cond));  
   printf("%ld rilascia il mutex per deallocare\n", pthread_self());         
  pthread_mutex_unlock(&(h->mutex));    
}

typedef struct {
    int mem;
    int sec;
    heap_t* heap;
} a_usr1_t;

void * usr1t_body(void * args){
    a_usr1_t* ausr1 = (a_usr1_t*) args;
    printf("Ciao sono %ld!\n", pthread_self());
    heap_alloc(ausr1->heap, ausr1->mem);
    printf("%ld ha finito di allocare e va a dormire per %d sec\n", pthread_self(), ausr1->sec);            
    sleep(ausr1->sec);
    printf("%ld si è svegliato\n", pthread_self());
    heap_dealloc(ausr1->heap, ausr1->mem);
     printf("Ciao sono %ld! E sono morto!\n", pthread_self());
    return NULL;
}

void handler(int s)
{
  printf("Segnale %d ricevuto dal processo %d\n", s, getpid());
  if(s==SIGUSR2) {
      printf("Dimensione HEAP: %d\n", heap->bytesLeft);
  } 
  if(s==SIGUSR1) {
    a_usr1_t args;
    pthread_t t;
    printf("MEM: ");
    scanf("%d", &(args.mem));
    assert(args.mem > 0);

    printf("SEC: ");
    scanf("%d", &(args.sec));
    assert(args.sec > 0);

    args.heap = heap;

    xpthread_create(&t, NULL, usr1t_body, &args,__LINE__,__FILE__);

  } 
}

#define MAX_BYTES 10000000

int main(int argc, char* argv[]){

    //inizializzo heap
    heap = (heap_t*) malloc(sizeof(heap_t));
    heap_init(heap, MAX_BYTES);

    // definisce signal handler 
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);     // setta a "insieme vuoto" sa.sa_mask maschera di segnali da bloccare 
    sa.sa_flags = SA_RESTART;     // restart system calls  if interrupted

    sigaction(SIGUSR1,&sa,NULL);  // handler per USR1
    sigaction(SIGUSR2,&sa,NULL);  // handler per USR2

    printf("PID: %d\n", getpid());
    while(true)
        pause();
    
    free(heap);

    return 0;
}