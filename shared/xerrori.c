#include "xerrori.h"

FILE *xfopen(const char *pathname, const char *mode, const char *file, const int line)
{
  FILE *f = fopen(pathname, mode);
  if (f == NULL)
  {
    perror("Errore apertura file");
    fprintf(stderr, "(%s:%d)\n", file, line);
    exit(1);
  }
  return f;
}

size_t xfwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream, const char *file, const int linea)
{

  size_t e = fwrite(ptr, size, nmemb, stream);
  if (e != nmemb)
  {
    die("Errore scrittura file", file, linea);
  }
  return e;
}

int xfclose(FILE *stream, const char *file, const int linea)
{
  int e = fclose(stream);
  if (e != 0)
  {
    die("Errore chiusura file", file, linea);
  }
  return e;
}

size_t xfread(void *ptr, size_t size, size_t nmemb, FILE *stream, const char *file, const int linea)
{
  size_t e = fread(ptr, size, nmemb, stream);
  if (e != nmemb)
  {
    die("Errore lettura file", file, linea);
  }
  return e;
}

pid_t xfork(const char *file, const int line)
{
  pid_t p = fork();
  if (p < 0)
  {
    die("ERRORE FORK", file, line);
  }
  return p;
}

pid_t xwait(int *status, const char *file, const int line)
{
  pid_t p = wait(status);
  if (p < 0)
  {
    die("ERRORE WAIT", file, line);
  }
  return p;
}

void die(const char *s, const char *file, const int line)
{
  perror(s);
  fprintf(stderr, "(%s,%d)\n", file, line);
  exit(1);
}