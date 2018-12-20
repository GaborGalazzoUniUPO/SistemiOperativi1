#include "xerrori.h"

/*************************************************************************************************/
/**************************      UTILS     *******************************************************/
/*************************************************************************************************/


void _die(const char *s, const char *file, const int line)
{
  printf("\033[0;31m"); 
  perror(s);
  fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),line,file);
  printf("\033[0m");
  exit(1);
}

/*************************************************************************************************/
/**************************      FILES     *******************************************************/
/*************************************************************************************************/

FILE *_xfopen(const char *pathname, const char *mode, const char *file, const int line)
{
  FILE *f = fopen(pathname, mode);
  if (f == NULL)
  {
    _die("Errore apertura file", file, line);
  }
  return f;
}

size_t _xfwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream, const char *file, const int linea)
{

  size_t e = fwrite(ptr, size, nmemb, stream);
  if (e != nmemb)
  {
    _die("Errore scrittura file", file, linea);
  }
  return e;
}

int _xfclose(FILE *stream, const char *file, const int linea)
{
  int e = fclose(stream);
  if (e != 0)
  {
    _die("Errore chiusura file", file, linea);
  }
  return e;
}

size_t _xfread(void *ptr, size_t size, size_t nmemb, FILE *stream, const char *file, const int linea)
{
  size_t e = fread(ptr, size, nmemb, stream);
  if (e != nmemb)
  {
    _die("Errore lettura file", file, linea);
  }
  return e;
}

int _xfseek(FILE *stream, long offset, int whence, const char *file, const int linea)
{
  int e = fseek(stream, offset, whence);
  if (e != 0)
  {
    _die("Errore fseek", file, linea);
  }
  return e;
}

long _xftell(FILE *stream, const char *file, const int linea)
{
  int e = ftell(stream);
  if (e < 0)
  {
    _die("Errore ftell", file, linea);
  }
  return e;
}

long _xfdim(FILE *stream, const char *file, const int linea)
{
  long old = _xftell(stream, file, linea);
  _xfseek(stream, 0, SEEK_END, file, linea);
  long size = _xftell(stream, file, linea);
  _xfseek(stream, old, SEEK_SET, file, linea);
  return size;
}

/**************************      SYSCALL     *****************************************************/

int _xclose(int fd, const char *file, const int linea)
{
  int e = close(fd);
  if (e < 0)
  {
    _die("Errore clode", file, linea);
  }
  return e;
}

ssize_t _xread(int fd, void *buf, size_t count, const char *file, const int linea)
{
  ssize_t e = read(fd, buf, count);
  if (e < 0)
  {
    _die("Errore read", file, linea);
  }
  return e;
}

ssize_t _xwrite(int fd, void *buf, size_t count, const char *file, const int linea)
{
  ssize_t e = write(fd, buf, count);
  if (e < 0)
  {
    _die("Errore write", file, linea);
  }
  return e;
}

/*************************************************************************************************/
/**************************      THREADS     *****************************************************/
/*************************************************************************************************/

pid_t _xfork(const char *file, const int line)
{
  pid_t p = fork();
  if (p < 0)
  {
    _die("ERRORE FORK", file, line);
  }
  return p;
}

pid_t _xwait(int *status, const char *file, const int line)
{
  pid_t p = wait(status);
  if (p < 0)
  {
    _die("ERRORE WAIT", file, line);
  }
  return p;
}

int _xpipe(int pipefd[2], const char *file, const int line) {
  int e = pipe(pipefd);
  if(e!=0) {
    _die("Errore creazione pipe", file, line); 
  }
  return e;
}

/*************************************************************************************************/
/**************************  SHARED MEMORY  *****************************************************/
/*************************************************************************************************/



int _xshm_open(const char *name, int oflag, mode_t mode, int linea, char *file)
{
  int e = shm_open(name, oflag, mode);
  if(e== -1) {
    _die("Errore shm_open", file, linea); 
  }
  return e;  
}

int _xshm_unlink(const char *name, int linea, char *file)
{
  int e = shm_unlink(name);
  if(e== -1) {
    _die("Errore shm_unlink", file, linea); 
  }
  return e;  
}

int _xftruncate(int fd, off_t length, int linea, char *file)
{
  int e = ftruncate(fd,length);
  if(e== -1) {
    _die("Errore ftruncate", file, linea); 
  }
  return e;  
}

void *_simple_mmap(size_t length, int fd, int linea, char *file)
{
  void *a =  mmap(NULL, length ,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
  if(a == (void *) -1) {
    _die("Errore mmap", file, linea); 
  }
  return a;
}

int _xmunmap(void *addr, size_t length, int linea, char *file)
{
  int e = munmap(addr, length);
  if(e== -1) {  
    _die("Errore munmap", file, linea); 
  }
  return e;
}

/*************************************************************************************************/
/**************************  SEMAPHORES *****************************************************/
/*************************************************************************************************/


int _xsem_init(sem_t *sem, int pshared, unsigned int value, int linea, char *file) {
  int e = sem_init(sem,pshared,value);
  if (e!=0) {
    _die("Errore sem_init", file, linea);
  }
  return e;
}

int _xsem_post(sem_t *sem, int linea, char *file) {
  int e = sem_post(sem);
  if (e!=0) {
    _die("Errore sem_post", file, linea);
  }
  return e;
}

int _xsem_wait(sem_t *sem, int linea, char *file) {
  int e = sem_wait(sem);
  if (e!=0) {
    _die("Errore sem_wait", file, linea);
  }
  return e;
}


/*************************************************************************************************/
/**************************  THREADS *************************************************************/
/*************************************************************************************************/



// ----- thread (non scrivono il codice d'errore in errno)
#define Buflen 100
#define Attesa 5 

// stampa il messaggio d'errore associato al codice en in maniera simile a perror
void _xperror(int en, char *msg) {
  char buf[Buflen];
  
  char *errmsg = strerror_r(en, buf, Buflen);
  if(msg!=NULL)
    fprintf(stderr,"%s: %s\n",msg, errmsg);
  else
    fprintf(stderr,"%s\n",errmsg);
}


int _xpthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg, int linea, char *file) {
  int e = pthread_create(thread, attr, start_routine, arg);
  if (e!=0) {
    _xperror(e, "Errore pthread_create");
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),linea,file);
    sleep(Attesa);  // si mette in attesa per non terminare subito gli altri thread 
    exit(1);
  }
  return e;                       
}

                          
int _xpthread_join(pthread_t thread, void **retval, int linea, char *file) {
  int e = pthread_join(thread, retval);
  if (e!=0) {
    _xperror(e, "Errore pthread_join");
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),linea,file);
    sleep(Attesa);  // si mette in attesa per non terminare subito gli altri thread 
    exit(1);
  }
  return e;
}

// mutex 
int _xpthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr, int linea, char *file) {
  int e = pthread_mutex_init(mutex, attr);
  if (e!=0) {
    _xperror(e, "Errore pthread_mutex_init");
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),linea,file);
    exit(1);
  }  
  return e;
}

int _xpthread_mutex_destroy(pthread_mutex_t *mutex, int linea, char *file) {
  int e = pthread_mutex_destroy(mutex);
  if (e!=0) {
    _xperror(e, "Errore pthread_mutex_destroy");
    fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),linea,file);
    exit(1);
  }
  return e;
}