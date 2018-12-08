#include "../shared/xerrori.h"

void scrivi1n(int n, char *nome)
{
    // apre file in scrittura
    FILE *f = xfopen(nome, "wb");
    assert(f != NULL);
    // scrive valori sul file
    for (int i = 1; i <= n; i++)
    {
        xfwrite(&i, sizeof(int), 1, f);
    }
    // chiude file
    xfclose(f);
    
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Uso:\n\t%s n nomefile\n\n", argv[0]);
        exit(1);
    }
    int n = atoi(argv[1]);
    if (n <= 0)
    {
        fprintf(stderr, "Inserisci un intero positivo\n");
        exit(1);
    }
    scrivi1n(n, argv[2]);
}
