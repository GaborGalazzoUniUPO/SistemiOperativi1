# definizione variabili
CFLAGS=-Wall -O -g -std=c99
CC=gcc
SHARED_H=$(wildcard ../shared/*.h)
SHARED_C=$(wildcard ../shared/*.c)


# elenco eseguibili
EXECS=sort_array

all: $(EXECS)

# regola generale per i file oggetto
%: %.c $(SHARED_H)
	$(CC) $(CFLAGS) -o $@ $< $(SHARED_C)

# utile per il docente per creare un archvio da mettere su DIR 
tarfile:
	tar -zcf lezione.tgz makefile *.c *.h *.py ../shared/*.h ../shared/*.c

# per quando si vuole ricominciare da zero 
clean:
	rm -f *.o $(EXECS)
