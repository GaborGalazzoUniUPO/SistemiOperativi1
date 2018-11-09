# definizione variabili
CFLAGS=-Wall -O -g -std=c99
CC=gcc

# elenco eseguibili
EXECS=contaprimi contaprimipipe contaprimifile

all: $(EXECS)

contaprimipipe: contaprimipipe.o ../shared/xerrori.o
	$(CC) $(CFLAGS) -o $@ $^

contaprimifile: contaprimifile.o ../shared/xerrori.o
	$(CC) $(CFLAGS) -o $@ $^


# regola generale per i file oggetto
%.o: %.c ../shared/xerrori.h
	$(CC) $(CFLAGS) -o $@ -c $<

# utile per il docente per creare un archvio da mettere su DIR 
tarfile:
	tar -zcf lezione.tgz makefile *.c *.h *.py ../shared/*.h ../shared/*.c

# per quando si vuole ricominciare da zero 
clean:
	rm -f *.o $(EXECS)
