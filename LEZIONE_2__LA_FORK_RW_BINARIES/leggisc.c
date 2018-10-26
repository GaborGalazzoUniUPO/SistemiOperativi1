#include "../shared/xerrori.h"
/**
 * Scrivere il programma leggisc che prende in input sulla linea di comando in nome di un file binario, 
 * legge gli interi in esso contenuti uno alla volta, e ne restituisce la somma. 
 * Il programma leggisc per aprire il file deve utilizzare la system call open e per leggere 
 * gli interi la system call read (man 2 read). Potete testare questo programma passandogli 
 * i file generati dal programma scrivi e ricordate che la somma degli interi tra 1 e n vale 
 * n(n+1)/2.
 */

void esercizio1(const char *filename)
{
    // apre file in scrittura
    int fd = open(filename, O_RDONLY, 0666);
    if (fd < 0)
        die("Errore creazione file", __FILE__, __LINE__);

    int i, acc = 0;
    while (read(fd, &i, sizeof(int)) == sizeof(int))
    {
        acc += i;
        assert(acc == (i * (i + 1) / 2));
    }

    printf("SOMMA: %d\n", acc);

    // chiude file
    int e = close(fd);
    if (e != 0)
        die("Errore chiusura file", __FILE__, __LINE__);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Uso:\n\t%s nomefile\n\n", argv[0]);
        exit(1);
    }
    esercizio1(argv[1]);
}