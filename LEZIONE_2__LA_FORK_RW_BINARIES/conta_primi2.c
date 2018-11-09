#include "../shared/xerrori.h"

/**
 * Scrivere un programma conta_primi2 per effettuare il conto dei primi compresi tra 1 e N 
 * utilizzando due processi. Il processo padre deve generare il processo figlio, 
 * contare i primi tra 1 e N/2 e attendere la terminazione del processo figlio. 
 * Il processo figlio deve contare il i primi tra N/2+1 e N e comunicare tale 
 * numero al padre mediante la funzione exit (stampare un warning se il numero 
 * dei primi trovati dal figlio sono più di 255 che e' il massimo valore che
 * può essere trasmesso mediante la exit). Utilizzare la seguente funzione 
 * per verificare se un intero è primo.
 */

bool primo(int n)
{
    int i;
    if (n < 2)
        return false;
    if (n % 2 == 0)
    {
        if (n == 2)
            return true;
        else
            return false;
    }
    for (i = 3; i * i <= n; i += 2)
    {
        if (n % i == 0)
        {
            return false;
        }
    }
    return true;
}

int conta_primi(int start, int end)
{
    int count = 0;
    for (int i = start; i <= end; i++)
    {
        if (primo(i))
            count++;
    }
    return count;
}

void esercizio3(int n)
{
    pid_t pid = xfork();
    if (pid == 0)
    { //FIGLIO
        exit(conta_primi(n / 2 + 1, n));
    }
    else
    { //PADRE

        int count = conta_primi(1, n / 2);
        int status;
        xwait(&status);
        if (WIFEXITED(status) >= 0)
        {
            count += WEXITSTATUS(status);
            printf("CONTEGGIO PRIMI: %d\n", count);
        }
        else
        {
            die("FIGLIO MORTO MALE");
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Uso:\n\t%s N\n\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]);
    if (n <= 0)
    {
        fprintf(stderr, "Inserisci un intero positivo\n");
        exit(1);
    }

    esercizio3(n);
}