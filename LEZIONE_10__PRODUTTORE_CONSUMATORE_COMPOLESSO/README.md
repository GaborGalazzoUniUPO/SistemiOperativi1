Lezione 10: Esercizio produttori/consumatori
--------------------------------------------

Scrivere un programma *divisoriT* che invocato dalla linea di comando scrivendo

```
divisoriT infile1 infile2 infile3 ... infileN outfile numt

```

con *numt>0* legge gli interi positivi contenuti nei file *infile1*, ... , *infileN*, e crea un file *outfile* contenente gli stessi interi e di fianco ad ogni intero il numero dei suoi divisori.

Il thread principale deve per prima cosa creare *N* thread produttori ad ognuno dei quali deve essere assegnato uno dei file di input. Ogni thread produttore deve leggere gli interi dal suo file, utilizzando ad esempio fscanf, e scriverli nel buffer produttori/consumatori condiviso. Ognuno di questi interi viene messo sul buffer produttore/consumatori e rappresenta una unità di lavoro per i consumatori. Il buffer deve avere una dimensione definita con *#define*.

Il thread principale deve successivamente aprire il file di ouput e creare *numt* thread consumatori ognuno dei quali deve ripetutamente:

-   leggere dal buffer un intero

-   calcolare il numero dei suoi divisori

-   scrivere nel file di output l'intero seguito dal numero dei suoi divisori (sulla stessa riga)

Come in tutti gli schemi produttore-consumatore è necessario stabilire un meccanismo che permetta ai produttori di segnalare ai consumatori che non ci sono altri valori da elaborare.

Si noti che tutti i consumatori scrivono sullo stesso file di output, è quindi necessario regolarne l'accesso mediante un mutex. Il file di output deve essere chiuso dal thread principale quando tutti i thread secondari sono terminati.

Esperimenti
-----------

Usare il programma *interi.py*, in fondo a questo file, per generare dei file di interi, e il programma *divisori.py* per generare il corrispondente file di divisori. Il vostro output puo' differire da quello prodotto da *divisori.py* per l'ordine delle righe; usare il comando `sort -n` per ottenere dei file con gli interi ordinati dal più piccolo al più grande.

```
#! /usr/bin/env python3

# programma interi.py
# crea un file di testo contenente un numero assegnato di interi positivi

import random
import sys

def main(n,nomefile):
  f = open(nomefile,"w");
  assert(f!=None);
  for i in range( n ):
    x = random.randint(1, 2**20)
    print(x,file=f)
  f.close();

if len(sys.argv)!=3:
  print("Uso:\n\t %s numero_interi outfile" % sys.argv[0])
else:
  main(int(sys.argv[1]), sys.argv[2] )

```

* * * * *

```
#! /usr/bin/env python3

# programma divisori.py
# calcola il numeri di divisori degli interi dati nel file di input

import sys

def main(infiles,outfile):
  g = open(outfile,"w");
  for infile in infiles:
    f = open(infile,"r");
    assert(f!=None and g!=None);
    for s in f:
      x = int(s)
      d = divisori(x)
      print(x,d,file=g)
    f.close();
  g.close();

def divisori( n ):
  assert(n>0)
  tot = 0
  for i in range ( 1,n+1 ):
    if n%i==0:
      tot += 1
  return tot

if len(sys.argv)<3:
  print("Uso:\n\t %s infile1 infile2 ... infile3 outfile" % sys.argv[0])
else:
  main(sys.argv[1:-1], sys.argv[-1] )

```

Tempo assegnato: 2 ore
----------------------