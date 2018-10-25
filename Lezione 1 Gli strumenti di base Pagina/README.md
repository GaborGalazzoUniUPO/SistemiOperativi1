Lezione 1: Gli strumenti di base
--------------------------------

Il comando *man*
----------------

-   Durante il corso utilizzeremo numerose nuove funzioni di libreria del C

-   Non è pensabile ricordarsi a memoria i dettagli di come funzionano,

-   E' necessario sapere usare il comando *man* che fornisce tutte le informazioni utili (dobbiamo però sapere il nome della funzione)

### Esempio: *man fread*

Eseguiamo il comando *man fread* sul terminale. Viene visualizzata l'informazione suddivisa in voci.

-   NAME indica il nome della/e funzioni a cui la pagina man si riferisce. In questo caso vediamo che la stessa pagina tratta delle funzioni fread e fwrite. Funzioni logicamente colleagate sono spesse presentate insieme

-   SYNOPSYS indica l'istruzione #include che deve essere messa nel file sorgente che usa tali funzioni. Successivamente ci sono i prototipi delle funzioni che ci dicono quali parametri devono essere forniti e in che ordine

-   DESCRIPTION indica il significato dei parametri e cosa fa la funzione
-   RETURN VALUE indica il significato del valore restituito dalla funzione. Le funzioni che utilizziamo spesso restituiscono un valore particolare per segnalare che si è verificato un errore. Grandi sciagure capitano a chi non controlla gli eventuali errori

Il compilatore
--------------

Il comando *gcc* non si limita a tradurre il nostro codice sorgente in codice eseguibile. Gran parte del suo lavoro consiste nel farci capire cosa abbiamo sbagliato nel codice attraverso *errori* o *warning*.

Ogni errore o warning è corredato da informazioni su cosa non è corretto: la lettura di tali messaggi permette di risparmiare *molto* tempo nella correzione degli errori.

I *warning* sono semplici avvertimenti di qualcosa che al compilatore appare sospetto; se ci sono solo warning l'eseguibile viene lo stesso creato e può essere eseguito, ma il 99% delle volte non è corretto.

MAI ignorare un warning di cui non si è capito completamente il significato

Affinché il compilatore ci dia il massimo di informazioni utili, compilare con i flag `-Wall -std=c99 -g -O`, ad esempio:

`gcc -Wall -std=c99 -g -O prog.c -o prog`

Il comando *make*
-----------------

Per programmi che consistono in più di un file sorgente le istruzioni di compilazione possono essere complicate e per questo motivo è comodo riassumerle in un unico file, chiamato *makefile*. Le istruzioni del makefile vengono eseguite dal comando *make*. Come sappiamo tale comando agisce in maniera intelligente eseguendo solamente il lavoro strettamente necessario.

### Esempio di makefile

```
# Il comando make cerca di realizzare il primo target (all)
# La linea "all: $(EXECS)" dice a make che tutti dipende
# tutti gli eseguibili e quindi il make provvedera' ad eseguire
# i comandi di compilazione

# opzioni di compilazione le definisco qui una volta per tutte
CFLAGS=-g -Wall -O -std=c99
LFLAGS=-lm

# definizione di variabile contenente i nomi degli eseguibili
EXECS=capitali ricorsione

all: $(EXECS)

# nelle regole di compilazione:
#  $@ viene sostituisto con il nome del target
#  $^ viene sostituito con tutte le dipendenze
#  $< viene sostituito con la prima dipendenza

capitali: capitali.o coordinate.o citta.o
    gcc $(CFLAGS) -o $@ capitali.o citta.o coordinate.o $(LFLAGS)

# Nella regola qua sotto il carattere % svolge il ruolo di wildcard (jolly)
# e quindi dice che un qualsiasi target (ad esempio persona) dipende dal file che
# si chiama come il target con estensione .c
# Quindi con una regola sola indichiamo che "persona" dipende da "persona.c"
# "persone" dipende da "persone.c" e così via.
%: %.c
    gcc $(CFLAGS) -o $@ $< $(LFLAGS)

# altro esempio di regola che usa %
%.o: %.c coordinate.h citta.h
    gcc $(CFLAGS) -o $@ -c $<

# target che si invoca per cancellar eseguibili e file oggetto
clean:
    rm -f $(EXECS) *.o

```

Quando le cose vanno male...
----------------------------

E' inevitabile fare degli errori quando si programma, oppure, il nostro programma è corretto ed è l'utente che lo usa male...

Per queste situazioni abbiamo visto a Programmazione 2 l'uso e le differenze fra `return`, `exit`, `assert`, e l'uso della funzione `perror`

Il tool *valgrind*
------------------

Una volta che un programma viene compilato correttamente non è detto che sia corretto, ci possono essere errori di progettazione o di realizzazione (ad esempio un `<`invece di un `<=`) che non sono rilevati dal compilatore e che spesso sono molto difficili da trovare anche perché spesso generano semplicemente un risultato sbagliato (o peggio un segmentation fault).

Uno strumento utile per trovare gli errori a runtime sono le asserzioni, un'altra possibilità è usare un debugger, ma in certi casi lo strumento più adatto è il tool *valgrind*.

```
Unfortunately, no one can be told what Valgrind is. You have to see it for yourself. (Morpheus)

```

Vedremo qualche esempio di uso di Valgrind in laboratorio, ma la cosa migliore per impararlo è farlo girare su un programma che non funziona seguendo la mini-guida disponibile su dir o la documentazionne ufficiale [http://www.valgrind.org](http://www.valgrind.org/)

Esercizio
---------

Abbiamo visto il programma *scrivi* che prende in input sulla linea di comando un intero *N* e il nome di un file *nome* e scrive in *nome* gli interi tra *1* e *N* in formato binario utilizzando 4 byte per intero.

1) Aggiungere nel file *xerrori.c* la funzione

```
 size_t xfwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream, const char *file, const int linea);

```

che analogamente a *xfopen* esegue l'operazione *fwrite* e in caso di errore visualizzi un opportuno messaggio e termini il programma. Modificare *scrivi.c* in modo che invochi *xfwrite* invece di *fwrite*.

2) Scrivere il programma *leggi* che prende in input sulla linea di comando in nome di un file binario e legge gli interi in esso contenuti e ne restituisce la somma. Il programma *leggi* per aprire il file deve utilizzare la funzione *xfopen* definita in *xerrori.c*. Modificare il makefile in modo che con il comando make vengano generati entrambi gli eseguibili *scrivi* e *leggi*