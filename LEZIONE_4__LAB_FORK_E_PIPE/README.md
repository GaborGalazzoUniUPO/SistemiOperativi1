Lezione 4: Laboratorio del 15/11/18 (fork e pipe)
-------------------------------------------------

**1)** L'esercizio consiste nell'ordinare un array di interi utilizzando due processi.

Il processo padre legge il numero di elementi *n* dell'array dalla linea di comando e costruisce un array di *n* elementi random compresi tra 0 e 1999. A tale scopo si puo' utilizzare la seguente funzione:

```
// genera array di n elementi con interi random tra 0 e 1999
int *random_array(int n)
{
  assert(n>0);
  int *a = malloc(n* sizeof(int));
  assert(a!=NULL);
  for(int i=0;i < n;i++)
    a[i] = (int) random() % 2000;
  return a;
}
```
Successivamente il padre crea un processo figlio. Il padre ha il compito di ordinare gli elementi minori di *1000* mentre il figlio ha il compito di ordinare i valori maggiori o uguali a *1000*. Entrambi i processi possono effettuare l'ordinamento con la funzione di libreria *qsort*.

Aiuto sull'uso del qsort La funzione da passare a *qsort* per fare il confronto di due interi è la seguente:
```
// funzione di confronto tra gli interi puntati da a e b
int intcmp(const void *a, const void *b)
{
  return *((int *) a) - *((int *) b);
}
```
Il figlio, dopo aver effettuato l'ordinamento, invia i valori ordinati al padre attraverso una pipe. Notare che il figlio puo' scrivere sulla pipe tutti i valori con una singola chiamata alla funzione *write*.

Il padre, dopo aver lanciato il processo figlio, ordina i valori minori di *1000* e successivamente legge i valori ordinati dal figlio scrivendoli nell'array di partenza che alla fine della lettura dovrebbe risultare ordinato. Dato che il padre sa quanti valori devono essere inviati dal figlio anch'esso puo' leggerli con una singola chiamata alla funzione *read*.

Per effettuare la verifica dell'ordinamento finale usate la seguente funzione:
```
// verifica che array a[0..n] sia ordinato
bool check_sort(int *a, int n)
{
  for(int i=0; i < n-1; i++)
     if(a[i] > a[i+1]) return false;
  return true;
}
```
Per ulteriore verifica della correttezza dell'algoritmo fate stampare la somma degli interi dell'array prima e dopo il sorting.

**Cercate di allocare la minore quantità di memoria possibile**

Suggerimento Per ottenere l'array da passare a *qsort* il padre può:

-   contare quanti sono gli elementi minori di 1000
-   allocare un array di quella dimensione
-   copiare nel nuovo array gli elementi minori di 1000
-   ordinare il nuovo array

Il figlio deve fare operazioni analoghe sugli elementi maggiori o uguali di 1000. Al termine ogni processo deve deallocare gli array utilizzati.
___
**2)** Consideriamo sempre il problema di ordinare un array utilizzando però una tecnica diversa vagamente ispirata al mergesort. Il padre genera due processi figli: il primo ordina gli interi nella prima meta' dell'array (posizioni da 0 a *n/2* escluso), il secondo quelli della seconda metà (da *n/2* a *n* escluso). I due processi figli ordinano le loro porzioni dell'array usando *qsort* come sopra, e al termine ognuno di loro invia i valori ordinati al padre usando una pipe dedicata.

Il processo padre, dopo aver lanciato i processi figli, legge dalle due pipe un valore alla volta. Non appena ha ottenuto un valore da entrambi i figli li confronta e scrive il più piccolo nell'array risultato e poi torna a leggere dalla pipe da cui ha preso il valore, ripetendo questo ciclo fino a quando non sono stati letti tutti ottenuti tutti i valori.

Ultime modifiche: giovedì, 15 novembre 2018, 09:27