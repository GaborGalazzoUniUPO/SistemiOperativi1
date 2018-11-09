Lezione 3: Creazione e uso di pipe
----------------------------------

### Argomenti trattati

-   Creazione e uso di pipe per scambio di dati tra processi.
-   Uso di pipe per sincronizzare i processi sfruttando il fatto che la read rimane in attesa che i dati arrivino sulla pipe.
-   Capacità di una pipe: attualmente in linux il massimo numero di byte che possono essere contenuti in una pipe è 64K. Raggiunto questo limite ulteriori scrittyre vengono messe in attesa.
-   Costante PIPE_BUF (attualmente su linux 4096). La scrittura e lettura di un numero di byte non superiore a questa soglia è garantita essere atomica: quando la scrittura è iniziata nessun altro processo accede alla pipe. Scritture di un numero maggiore di byte potrebbero non essere atomiche: se due processi scrivono contemporaneamente sulla pipe i loro dati potrebbero essere inframezzati nella pipe.

### Esercizio per casa

Scrivere un programma *dividiP* che invocato dalla linea di comando scrivendo

```
dividiP nomefile

```

dove `nomefile` è un file di testo contenente interi, crea due file `nomefile.pari` e `nomefile.dispari` contenenti rispettivamente i valori pari e dispari di `nomefile`. Il programma deve seguire il seguente procedimento. Il processo principale deve creare due processi secondari, indicati nel seguito con p0 e p1, e con ognuno di essi deve comunicare attraverso una pipe distinta.

Il processo principale legge un intero alla volta da `nomefile`, se l'intero è pari lo scrive nella pipe di p0, se e' dispari lo scrive nella pipe di p1. Contemporaneamente, p0 crea il file `nomefile.pari` e ci scrive i valori letti dalla sua pipe, mentre p1 crea il file `nomefile.dispari` e ci scrive i valori letti dalla sua pipe, realizzando così la divisione in pari e dispari richiesta.

Terminata la lettura/scrittura dei dati i processi secondari devono chiudere il file che hanno creato e stampare sul terminale la somma dei numeri che hanno ricevuto.

Per fare test significativi potete usare il programma *paridis.py* qui sotto che crea un file con un numero assegnato di interi e stampa i valori che deve essere visualizzati dal vostro programmi (la somma degli interi pari e dispari). Per eseguire questo programma scrivere `python3 paridis.py` sul terminale.

### Programma paridis.py

```
#! /usr/bin/env python3
import random
import sys

def main(n,nomefile):
  f = open(nomefile,"w")
  assert(f!=None)
  totp = totd = 0
  for i in range( n ):
    x = random.randint(-100, 100)
    print(x,file=f)
    if x%2==0:
      totp += x
    else:
      totd += x
  f.close()
  print("Somma pari:", totp, "somma dispari:", totd)

if len(sys.argv)!=3:
  print("Uso:\n\t %s numero_interi file" % sys.argv[0])
else:
  main(int(sys.argv[1]), sys.argv[2] )

```

Ultime modifiche: venerdì, 9 novembre 2018, 11:37