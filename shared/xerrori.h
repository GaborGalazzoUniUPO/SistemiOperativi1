#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>  // gestisce tipo bool (per variabili booleane)
#include <assert.h>   // permette di usare la funzione assert
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */


/*************************************************************************************************/
/**************************      UTILS     *******************************************************/
/*************************************************************************************************/

void _die(const char *s, const char *file, const int line);

#define die(s) _die(s, __FILE__,__LINE__)

typedef char* String;

/*************************************************************************************************/
/**************************      FILES     *******************************************************/
/*************************************************************************************************/

FILE *_xfopen(const char *pathname, const char *mode, const char *file, const int line);
int _xfclose(FILE *stream, const char *file, const int linea);
size_t _xfwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream, const char *file, const int linea);
size_t _xfread(void *ptr, size_t size, size_t nmemb, FILE *stream, const char *file, const int linea);
int _xfseek(FILE *stream, long offset, int whence, const char *file, const int linea);
long _xftell(FILE *stream, const char *file, const int linea);
long _xfdim(FILE *stream, const char *file, const int linea);

#define xfopen(pathname, mode) _xfopen(pathname, mode, __FILE__, __LINE__)
#define xfclose(stream) _xfclose(stream, __FILE__, __LINE__)
#define xfwrite(ptr, size, nmemb, fp) _xfwrite(ptr, size, nmemb, fp, __FILE__, __LINE__)
#define xfread(ptr, size, nmemb, fp) _xfread(ptr, size, nmemb, fp, __FILE__, __LINE__)
#define xfseek(stream, offset, whence) _xfseek(stream, offset, nmemb, __FILE__, __LINE__)
#define xftell(stream) _xftell(stream, __FILE__, __LINE__)
#define xfdim(stream) _xfdim(stream, __FILE__, __LINE__)

/***************** SYSCALL ***********************************************************************/

int _xclose(int fd, const char *file, const int linea);
ssize_t _xread(int fd, void *buf, size_t count, const char *file, const int linea);
ssize_t _xwrite(int fd, void *buf, size_t count, const char *file, const int linea);

#define xclose(fd) _xclose(fd, __FILE__,__LINE__)
#define xread(fd, buf, count) _xread(fd, buf, count, __FILE__,__LINE__)
#define xwrite(fd, buf, count) _xwrite(fd, buf, count, __FILE__,__LINE__)

/*************************************************************************************************/
/**************************      PROCESS     *****************************************************/
/*************************************************************************************************/

pid_t _xfork(const char *file, const int line);
pid_t _xwait(int *status, const char *file, const int line);
int _xpipe(int pipefd[2], const char *file, const int line) ;

#define xfork() _xfork(__FILE__, __LINE__)
#define xwait(status) _xwait(status, __FILE__, __LINE__)
#define xpipe(pipefd) _xpipe(pipefd, __FILE__,__LINE__)

/*************************************************************************************************/
/**************************  SHARED MEMORY  *****************************************************/
/*************************************************************************************************/

int _xshm_open(const char *name, int oflag, mode_t mode, int linea, char *file);
int _xshm_unlink(const char *name, int linea, char *file);
int _xftruncate(int fd, off_t length, int linea, char *file);
void* _simple_mmap(size_t length, int fd, int linea, char *file);
int _xmunmap(void *addr, size_t length, int linea, char *file);


#define xshm_open(name, oflag, mode) _xshm_open(name, oflag, mode, __LINE__,__FILE__)
#define xshm_unlink(name) _xshm_unlink(name, __LINE__,__FILE__)
#define xftruncate(fd, length) _xftruncate(fd, length, __LINE__,__FILE__)
#define simple_mmap(length, fd) _simple_mmap(length, fd, __LINE__,__FILE__)
#define xmunmap(addr, length) _xmunmap(addr, length, __LINE__,__FILE__)

/*************************************************************************************************/
/**************************  SEMAPHORES *****************************************************/
/*************************************************************************************************/


int _xsem_init(sem_t *sem, int pshared, unsigned int value, int linea, char *file);
int _xsem_post(sem_t *sem, int linea, char *file);
int _xsem_wait(sem_t *sem, int linea, char *file);

#define xsem_init(sem, pshared, value) _xsem_init(sem, pshared, value, __LINE__,__FILE__)
#define xsem_post(sem) _xsem_post(sem, __LINE__,__FILE__) 
#define xsem_wait(sem) _xsem_wait(sem, __LINE__,__FILE__) 


/*************************************************************************************************/
/**************************  THREADS *************************************************************/
/*************************************************************************************************/


void _xperror(int en, char *msg);
int _xpthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg, int linea, char *file);
int _xpthread_join(pthread_t thread, void **retval, int linea, char *file);

#define xperror(en, msg) _xperror(en, msg, __LINE__, __FILE__)
#define xpthread_create(thread, attr, start_routine, arg) _xpthread_create(thread, attr, start_routine, arg, __LINE__,__FILE__)
#define xpthread_join(thread, retval) _xpthread_join(thread, retval, __LINE__, __FILE__)


// mutex 
int _xpthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr, int linea, char *file);
int _xpthread_mutex_destroy(pthread_mutex_t *mutex, int linea, char *file);

#define xpthread_mutex_init(mutex,attr) xpthread_mutex_init(mutex,attr, __LINE__,__FILE__) 
#define xpthread_mutex_destroy(mutex) _xpthread_mutex_destroy(mutex, __LINE__,__FILE__) 
