# definizione variabili
CFLAGS=-Wall -O -g -std=c99
CC=gcc

# elenco eseguibili
EXECS=scrivi scrivi_shared leggi

all: $(EXECS)

scrivi: scrivi.o ../shared/xerrori.o
	$(CC) $(CFLAGS) -o $@ $^

leggi: leggi.o ../shared/xerrori.o
	$(CC) $(CFLAGS) -o $@ $^

scrivi_shared: scrivi_shared.o ../shared/xerrori.o
	$(CC) $(CFLAGS) -o $@ $^

# regola generale per i file oggetto
%.o: %.c ../shared/xerrori.h
	$(CC) $(CFLAGS) -o $@ -c $<

# utile per il docente per creare un archvio da mettere su DIR 
tarfile:
	tar -zcf lezione.tgz makefile *.c *.h

# per quando si vuole ricominciare da zero 
clean:
	rm -f *.o $(EXECS)
