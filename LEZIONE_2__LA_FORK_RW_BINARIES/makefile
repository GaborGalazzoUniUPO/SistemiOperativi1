# definizione variabili
CFLAGS=-Wall -O -g -std=c99
CC=gcc

# elenco eseguibili
EXECS=fork scrivisc leggisc leggisc_smart

all: $(EXECS)

fork: fork.o ../shared/xerrori.o
	$(CC) $(CFLAGS) -o $@ $^

scrivisc: scrivisc.o ../shared/xerrori.o
	$(CC) $(CFLAGS) -o $@ $^

leggisc: leggisc.o ../shared/xerrori.o
	$(CC) $(CFLAGS) -o $@ $^

leggisc_smart: leggisc_smart.o ../shared/xerrori.o
	$(CC) $(CFLAGS) -o $@ $^

# regola generale per i file oggetto
%.o: %.c ../shared/xerrori.h
	$(CC) $(CFLAGS) -o $@ -c $<

# utile per il docente per creare un archvio da mettere su DIR 
tarfile:
	tar -zcf lezione.tgz makefile *.c *.h ../shared/*

# per quando si vuole ricominciare da zero 
clean:
	rm -f *.o $(EXECS)
