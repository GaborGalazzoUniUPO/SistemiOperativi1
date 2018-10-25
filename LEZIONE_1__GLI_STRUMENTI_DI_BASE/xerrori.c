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

