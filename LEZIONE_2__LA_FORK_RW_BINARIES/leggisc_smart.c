#include "../shared/xerrori.h"
/**
 * Scrivere il programma leggisc_smart che prende in input sulla linea di comando in nome di un file binario, 
 * legge contemporaneamente tutti gli interi in esso contenuti e ne restituisce la somma. 
 * Il programma leggisc_smart per aprire il file deve utilizzare la system call 
 * open e per leggere gli interi deve chiamare una sola volta la system call read. 
 * Ovviamente prima di tale chiamata deve allocare un buffer grande quanto il file;
 * utilizzare la system call lseek per ottenere la dimensione del file.
 * */

int somma_array_seq(int *array, int n)
{
    int acc = 0;
    for (int i = 0; i < n; i++)
    {
        assert(acc == (i * (i + 1) / 2));
        acc += array[i];
    }
    assert(acc == (n * (n + 1) / 2));
    return acc;
}

void esercizio2(const char *filename)
{
    // apre file in scrittura
    int fd = open(filename, O_RDONLY, 0666);
    if (fd < 0)
        die("Errore creazione file", __FILE__, __LINE__);

    off_t bytes = lseek(fd, 0, SEEK_END);
    if (bytes < 0)
        die("ERROR POSITIONING POINTER TO EOF", __FILE__, __LINE__);
    if (lseek(fd, 0, SEEK_SET) < 0)
        die("ERROR POSITIONING POINTER TO START", __FILE__, __LINE__);

    int array[bytes / sizeof(int)];
    ssize_t e0 = read(fd, &array, bytes);
    if (e0 != bytes)
        die("ERROR READING", __FILE__, __LINE__);

    printf("SOMMA: %d\n", somma_array_seq(array, bytes / sizeof(int)));

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
    esercizio2(argv[1]);
}