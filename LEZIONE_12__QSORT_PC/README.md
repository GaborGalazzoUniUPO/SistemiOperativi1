Laboratorio del 20/12: quicksort con produttori e consumatori
-------------------------------------------------------------

Il file *[quicksort](https://www.dir.uniupo.it/mod/resource/view.php?id=184885 "quicksort").c* contiene una classica implementazione del [quicksort](https://www.dir.uniupo.it/mod/resource/view.php?id=184885 "quicksort") con aggiunta solo una verifica finale che l'ordinamento sia stato effettuato correttamente (utilizzando l'array *copia*).

L'esercizio di oggi consiste nel trasformare questo algoritmo in una versione che sfrutta i thread utilizzando il paradigma producer/consumer. Il thread principale svolge il ruolo di producer; esegue il [quicksort](https://www.dir.uniupo.it/mod/resource/view.php?id=184885 "quicksort") ma non appena si trova a ordinare una porzione dell'array di dimensione inferiore a *limite* (parametro fornito sulla linea di comando) provvede a creare una unità di lavoro per il pool dei consumer uno dei quali si incaricherà del lavoro e provvederà a ordinare quella porzione dell'array.

Il programma deve essere invocato con la linea di comando

veryquicksort numel numth limite [seed]

dove *numel* e' il numero di elementi nell'array, *numth* è il numero di thread consumer, *limite* è il la dimensione masssima dei blocchi assegnata ai worker e *seed* è un parametro opzionale che puo' essere usato come seed per i numeri casuali (altrimenti in tutti gli esperimenti ordinate sempre lo stesso array). Fate riferimento a *[quicksort](https://www.dir.uniupo.it/mod/resource/view.php?id=184885 "quicksort").c* per vedere la gestione di *seed*.

### Schema di risoluzione

thread principale:
  crea il buffer e i semafori per la sua gestione
  crea p thread consumer
  esegue very_quicksort(A,0,n-1)
  aspetta che tutti i consumer abbiano finito
  controlla che l'array sia stato ordinato correttamente

La procedura *very_quicksort* eseguita dal thread principale ha la seguente struttura:

```

void very_quicksort(A,p,r,...)
  if(p < r) {
    if (numero di elementi da ordinare <= limite)
      scrivi coppia (p,q) sul buffer
    else {
      // esegui come nel caso senza thread:
      q =  partition(A,p,r);
      ordina ricorsivamente A[p] ... A[q]
      ordina ricorsivamente A[q+1] .. A[r]
    }
  }

```

Ho usato i puntini di sospensione per gli argomenti di *very_quicksort* perché è necessario passargli altri elementi oltre a *A*, *p*, *r*: sicuramente *limite* ma anche tutto ciò che serve per mettere gli elementi sul buffer.

I thread consumer devono eseguire tutti il seguente ciclo

while(1) {
  aspetta che compaia una coppia (p,r) sul buffer
  [quicksort](https://www.dir.uniupo.it/mod/resource/view.php?id=184885 "quicksort")(A,p,r)
}

Esercizio per casa (complicato, ma avete le vacanze a disposizione)
-------------------------------------------------------------------

Scrivere una versione multithread del [quicksort](https://www.dir.uniupo.it/mod/resource/view.php?id=184885 "quicksort") che combina insieme le tecniche di questo esercizio e quelle viste nell'esercizio 2 del 6/12/18. In pratica la procedura *very_quicksort* vista sopra deve essere modificata in modo da realizzare la seguente strategia:

```

void very_quicksort(A,p,r,...)
  if(p < r) {
    if (numero di elementi da ordinare <= limite)
      scrivi coppia (p,q) sul buffer
    else {
      // molti elementi da ordinare: fatti aiutare da un thread ausiliario
      q =  partition(A,p,r);
      genera un thread taux e fagli eseguire very_quicksort(A,p,q,...)
      contemporaneamente esegui very_quicksort(A,q+1,r,...)
      aspetta terminazione thread taux e termina
    }
  }

```

La difficoltà dell'esercizio nasce dal fatto che i thread *taux* a loro volta scriveranno coppie *(p,q)* sul buffer e quindi saremo in una situazione con molti thread consumatori (non è possibile stabilire a priori quanti) e *numth* thread consumatori.

Ultime modifiche: mercoledì, 19 dicembre 2018, 18:02