# definizione variabili
CFLAGS=-Wall -O -g -std=c99
CC=gcc
LDFLAGS=-pthread -lrt
SHARED_H=$(wildcard ../shared/*.h)
SHARED_C=$(wildcard ../shared/*.c)


# elenco eseguibili
EXECS=mt_mergesort mt_qsort

all: $(EXECS)

# regola generale per i file oggetto
%: %.c $(SHARED_H)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<  $(SHARED_C) 

# utile per il docente per creare un archvio da mettere su DIR 
tarfile:
	tar -zcf lezione.tgz makefile *.c *.h *.py $(SHARED_H) $(SHARED_c)

# per quando si vuole ricominciare da zero 
clean:
	rm -f *.o $(EXECS)
