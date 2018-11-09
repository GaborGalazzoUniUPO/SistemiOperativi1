#include "xerrori.h"

/*************************************************************************************************/
/**************************      UTILS     *******************************************************/
/*************************************************************************************************/

void print_trace(void)
{
  void *buffer[200];
  int n;

  n = backtrace(buffer, 200);
  fprintf(stderr, "--- (depth %d) ---\n", n);
  backtrace_symbols_fd(buffer, n, STDERR_FILENO);
}

void _die(const char *s, const char *file, const int line)
{
  perror(s);
  print_trace();
  fprintf(stderr,"== %d == Linea: %d, File: %s\n",getpid(),line,file);
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
