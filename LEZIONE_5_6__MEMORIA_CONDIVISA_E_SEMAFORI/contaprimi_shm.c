#include "../shared/xerrori.h"

// restituisce true/false a seconda che n sia primo o composto
bool primo(int n)
{
  int i;
  if(n<2) return false;
  if(n%2==0) {
    if(n==2)
      return true;
    else
      return false;
  }
  for (i=3; i*i<=n; i += 2) {
      if(n%i==0) {
          return false;
      }
  }
  return true;
}

void* create_buffer(int n, char* name, size_t dim){
  int shm_size = n*dim; // numero di byte necessari
  int fd = xshm_open(name,O_RDWR | O_CREAT, 0600);
  xftruncate(fd, shm_size);
  void *a = simple_mmap(shm_size,fd);
  close(fd); // dopo mmap e' possibile chiudere il file descriptor
  return a;
}

sem_t* create_ssem(char* name, int init){
    sem_t* sem = create_buffer(1, name, sizeof(sem_t));
    xsem_init(sem,1,init);
    return sem;
}

int main(int argc, char* argv[]){

    if(argc!=3) {
        printf("Uso:\n\t%s nomefile p\n",argv[0]);
        exit(1);
    }
    puts("");

    int p = atoi(argv[2]);
    assert(p>0);

    // array condiviso int buffer[10] di 10 interi 
    // un intero condiviso num che dice quanti valori 
    // da testare sono attualmente presenti nel buffer
    int* sum = create_buffer(14, "/buffer", sizeof(int));
    int* end = sum +1;
    int* num = end +1;
    int* buffer = num+1;
    *num = 0;
    *end = 0;
    *sum = 0;

    // semaforo sem_buffer usato per garantire che solo 
    // un processo alla volta possa accedere alle variabili buffer e num
    sem_t* sem_buffer = create_ssem("/mutex", 1);

   
    for(int i=0;i<p;i++) {
        pid_t pid = xfork();
        if(pid==0) {
            while(true){

                while(true){
                    // tenta di accedere al semaforo sem_buffer
                    xsem_wait(sem_buffer);
                    
                    // Una volta ottenuto l'accesso controlla num:
                    if(*num > 0)    // se num>0 il buffer non è vuoto
                        break;      // procede con l'esecuzione

                    int t_end = *end;
                    xsem_post(sem_buffer);
                    if(t_end)
                    {
                        xsem_wait(sem_buffer);
                        xsem_post(sem_buffer);
                        xmunmap(sum,13*sizeof(int));
                        xmunmap(sem_buffer,1*sizeof(sem_t));
                        exit(EXIT_SUCCESS);
                    }

                    // il processo figlio esegue una sleep 
                    // per un secondo e successivamente riprova la stessa operazione.
                    usleep(100);
                }
               
                //decrementa num 
                (*num)--;
                 //l figlio legge il valore buffer[num-1],
                int val = buffer[(*num)];
                if(primo(val))
                    (*sum)++;
                xsem_post(sem_buffer);
                if(val == EOF) // SE IL BUFFER CONTIENE -1 ESCO
                    break;

                
            }
        }  
    }

    // i padre inizia a leggere gli interi dal file
    FILE *f = xfopen(argv[1],"rt");
    

    while(true) {
        // il processo padre deve leggere dal file un intero 
        // e cercare di scriverlo in buffer
        int x;
        int e = fscanf(f,"%d",&x);

        
        while(true){
            // tenta di accedere al semaforo sem_buffer
            xsem_wait(sem_buffer);
            // Una volta ottenuto l'accesso controlla num:
            if(*num <= 10)  // se num==10 il buffer è pieno e il processo 
                break;      // padre non può aggiungere il valore; 
            xsem_post(sem_buffer);
            // di conseguenza il padre esegue una sleep 
            // per un secondo e successivamente riprova la stessa operazione.
            usleep(100);
        }
        // se num<10 il padre aggiunge il valore al buffer
        // incrementa num e passa alla 
        // lettura del valore successivo da file

        if(e==0|| e==EOF){
            *end = 1;
        }
        else{
            buffer[*num] = x;
            (*num)++;
        }
        xsem_post(sem_buffer);
        if(e==0|| e==EOF) break;
    }
    fclose(f);

    for(int i = 0; i<p; i++){
        xwait(NULL);
    }

    printf("SOMMA: %d\n", (*sum));

    xmunmap(sum,13*sizeof(int));
    xmunmap(sem_buffer,1*sizeof(sem_t));
    xshm_unlink("/mutex");
    xshm_unlink("/buffer");

}