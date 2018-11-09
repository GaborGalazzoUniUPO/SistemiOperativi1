#define _GNU_SOURCE  // permette di usare estensioni GNU
#include <stdio.h>   // permette di usare scanf printf ..
#include <stdlib.h>  // conversioni stringa/numero rand() abs() exit() etc ...
#include <stdbool.h> // gestisce tipo bool (per variabili booleane)
#include <assert.h>  // permette di usare la funzione assert

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <execinfo.h>

/*************************************************************************************************/
/**************************      UTILS     *******************************************************/
/*************************************************************************************************/

void _die(const char *s, const char *file, const int line);

#define die(s) _die(s, __FILE__,__LINE__)

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

/*************************************************************************************************/
/**************************      PROCESS     *****************************************************/
/*************************************************************************************************/

pid_t _xfork(const char *file, const int line);
pid_t _xwait(int *status, const char *file, const int line);
int _xpipe(int pipefd[2], const char *file, const int line) ;

#define xfork() _xfork(__FILE__, __LINE__)
#define xwait(status) _xwait(status, __FILE__, __LINE__)
#define xpipe(pipefd) _xpipe(pipefd, __FILE__,__LINE__)
