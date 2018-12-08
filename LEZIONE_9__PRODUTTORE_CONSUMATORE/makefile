# definizione variabili
CFLAGS=-Wall -std=c99 -g -O
LDFLAGS=-pthread -lrt
CC=gcc
SHARED_H=$(wildcard ../shared/*.h)
SHARED_C=$(wildcard ../shared/*.c)


# elenco eseguibili
EXECS=incompleto convertiT

all: $(EXECS)

# regola generale per i file oggetto
%: %.c $(SHARED_H)
	$(CC) $(CFLAGS) -o $@ $<  $(SHARED_C)  $(LDFLAGS)

# utile per il docente per creare un archvio da mettere su DIR 
tarfile:
	tar -zcf lezione.tgz makefile *.c *.h *.py $(SHARED_H) $(SHARED_c)

# per quando si vuole ricominciare da zero 
clean:
	rm -f *.o $(EXECS)
