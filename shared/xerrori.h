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

// prototipi di xerrori.c

FILE *xfopen(const char *pathname, const char *mode, const char *file, const int line);

size_t xfwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream, const char *file, const int linea);

size_t xfread(void *ptr, size_t size, size_t nmemb, FILE *stream, const char *file, const int linea);

int xfclose(FILE *stream, const char *file, const int linea);

void die(const char *s, const char *file, const int line);

pid_t xfork(const char *file, const int line);

pid_t xwait(int *status, const char *file, const int line);
