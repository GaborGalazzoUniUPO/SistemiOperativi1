#include "xerrori.h"

FILE *xfopen(const char *pathname, const char *mode, const char *file, const int line) {
  FILE *f = fopen(pathname,mode);
  if(f==NULL) {
    perror("Errore apertura file");
    fprintf(stderr,"(%s:%d)\n",file,line);
    exit(1);
  }
  return f;
}

pid_t xfork(const char *file, const int line){
  pid_t p =  fork();
  if(p<0){
    die("ERRORE FORK", file, line);
  }
  return  p;
}

void die(const char *s, const char *file, const int line){
  perror(s);
  fprintf(stderr, "(%s,%d)\n", file, line);
  exit(1);
}