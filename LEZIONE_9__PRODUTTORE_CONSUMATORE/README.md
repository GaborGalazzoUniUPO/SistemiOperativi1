Lezione 9: tecnica produttore consumatore
-----------------------------------------

### Argomenti trattati

-   Abbiamo parlato della difficoltà di sfruttare la potenze delle macchine multicore suddividendo e coordinando la computazione in maniera bilanciata tra diversi thread

-   Come soluzione relativamente semplice ed efficiente a questo problema abbiamo illustrato la tecnica produttori/consumatori in cui la computazione viene divisa in unità più piccole da uno o più produttori. Ogni unità di computazione viene gestita da uno o più consumatori

-   Abbiamo mostrato che questa tecnica richiede l'uso di un buffer circolare per la comunicazione tra produttori e consumatori e di *due* semafori per la loro sincronizzazione. Vederemo nella prossima lezione l'utilizzo dei mutex per risolvere i conflitti fra i produttori (se sono più di uno) o i consumatori (se sono più di uno).

-   Un altro ingrediente importante è meccanismo che permetta ai produttori di segnalare ai consumatori che non ci sono altre computazioni da effettuare.

-   Il sorgente *incompleto.c* mostra una applicazione completa della tecnica produttore/consumatori. Non e' completo in quanto se ci sono più di due consumatori ci possono essere dei conflitti sull'accesso alla variabile *cindex*.

-   Per casa, completare l'esercizio aggiungendo un semaforo binario che permetta ad un solo consumatore alla volta l'accesso a *cindex*. Con questa modifica il risultato dovrebbe essere corretto per qualunque numero di thread utilizzati. Utilizzare il comando `python3 numeri.py` per generare dei file "grandi" su cui testare l'algoritmo.

-   Per casa fate anche l'Esercizio 1 del compito del 18/7/2017 nel quale si utilizzano due buffer produttore/consumatore, ognuno con un un solo produttore e un solo consumatore.

Ultime modifiche: giovedì, 6 dicembre 2018, 20:17

Esercizio 1 (threads)
---------------------

Scrivere un programma *convertiT* che invocato dalla linea di comando scrivendo

  convertiT infile outfile

legge il contenuto di *infile* e lo scrive in *outfile* dopo aver convertito le lettere minuscole in maiuscole e viceversa. La conversione deve essere effettuata utilizzando due thread ausiliari *t2* e *t3* secondo il seguente schema. Il thread principale legge una riga alla volta da *infile* copiandola in una stringa opportunamente allocata. Il puntatore a questa stringa viene passato al thread *t2* che esegue la conversione di maiuscole e minuscole e che la passa al thread *t3*. Il thread *t3* scrive la stringa trasformata su *outfile* e dealloca lo spazio utilizzato dalla stringa.

Il passaggio delle stringhe tra i vari thread deve essere effettuato mediante il meccanismo produttore/consumatore. Avremo quindi due di tali meccanismi (quindi due buffer) ognuno con un unico produttore e un unico consumatore: notare che il thread *t2* svolge il ruolo di consumatore rispetto al thread principale e il ruolo di produttore rispetto al thread *t3*.

Nota: per testare se un carattere è maiuscolo o minuscolo usare le funzioni *isupper* e *islower*. Per convertire un carattere in maiuscolo o minuscolo usare le funzioni *toupper* e *tolower*.

Si definiscano due costanti di compilazione

#define Buf_size 10
#define Max_line_len 100

che indicano la dimensione dei buffer produttore/consumatore e la massima lunghezza din una linea del file di testo.