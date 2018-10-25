Mini guida al tool valgrind
---------------------------

Una delle difficoltà del C è il fatto che durante l'esecuzione del programma non vengono generati messaggi d'errore molto informativi. Tipicamente otteniamo un risultato sbagliato o addirittura semplicemente *segmentation fault*.

Durante il corso abbiamo visto come affrontare questo problema usando le asserzioni, o eventualmente delle stampe di controllo, ma queste non risolvono sempre il problema. Un altro tool importante, sopratutto per quanto riguarda l'uso degli array, è *valgrind* che chi programma seriamente in C deve conoscere.

*valgrind* è un tool non facilissimo da usare: vediamo ora un esempio di un suo utilizzo elementare; andando avanti negli studi potrete utilizzarlo in maniera più sofisticata.

Consideriamo il programma *[testvgrind](https://www.dir.uniupo.it/mod/resource/view.php?id=184862 "testvgrind").c* e compiliamolo con

  gcc -g -O -Wall -std=c99 -o [testvgrind](https://www.dir.uniupo.it/mod/resource/view.php?id=184862 "testvgrind") [testvgrind](https://www.dir.uniupo.it/mod/resource/view.php?id=184862 "testvgrind").c

(notate l'uso anche dello switch *-g* che e' importante). Il programma riempie un array di 1 e calcola la somma degli elementi. Se la somma risulta uguale al numero di elementi stampa *giusto* altrimenti stampa *sbagliato*.

Se eseguiamo

    [testvgrind](https://www.dir.uniupo.it/mod/resource/view.php?id=184862 "testvgrind") 5

otteniamo la scritta "sbagliato" che indica che c'e' un errore nel programma.

Se, senza modificare ne ricompilare il programma, scriviamo

  valgrind [testvgrind](https://www.dir.uniupo.it/mod/resource/view.php?id=184862 "testvgrind") 5

l'esecuzione del programma viene monitorata da *valgrind* che segnala sullo schermo dei possibili errori. Nel nostro esempio, un primo messaggio dirà qualcosa del tipo (gli indirizzi saranno diversi per ogni esecuzione)

==9028== Invalid write of size 4
==9028==    at 0x40077B: riempi ([testvgrind](https://www.dir.uniupo.it/mod/resource/view.php?id=184862 "testvgrind").c:49)
==9028==    by 0x4006F8: main ([testvgrind](https://www.dir.uniupo.it/mod/resource/view.php?id=184862 "testvgrind").c:29)
==9028==  Address 0x51fa054 is 0 bytes after a block of size 20 alloc'd
==9028==    at 0x4C2BBAD: malloc (vg_replace_malloc.c:299)
==9028==    by 0x4006C3: main ([testvgrind](https://www.dir.uniupo.it/mod/resource/view.php?id=184862 "testvgrind").c:25)

Il messaggio qui sopra dice che alla riga 49 (testgrind.c:49) sono stati scritti 4 byte (quindi un int) in una posizione "0 byte dopo un blocco di 20" allocato alla riga 25. Quindi abbiamo scritto in una posizione subito dopo la fine dell'array *a* che avevamo allocato con malloc alla linea 25. A questo punto abbiamo capito che dentro la funzione *riempi* andiamo a scrivere oltre la fine dell'array, e il motivo è che abbiamo usato il minore uguale invece del minore nel ciclo for.

Un secondo messaggio d'errore di valgrind dice:

==9028== Conditional jump or move depends on uninitialised value(s)
==9028==    at 0x400734: main ([testvgrind](https://www.dir.uniupo.it/mod/resource/view.php?id=184862 "testvgrind").c:37)

che tradotto vuol dire che il risultato del test alla linea 37 dipende da un valore non inizializzato. Dato che *n* è stato inizializzato il problema deve essere nella variabile *tot*, che viene inizializzata con la somma degli elementi dell'array *a*. E' possibile che uno degli elementi di *a* non sia stato inizializzato? Guardando *riempi* capiamo che il ciclo, partendo da 1, non inizializza l'elemento *a[0]*.

Valgrind termina mostrando un riassunto sull'uso dell'heap:

==9028== HEAP SUMMARY:
==9028==     in use at exit: 20 bytes in 1 blocks
==9028==   total heap usage: 2 allocs, 1 frees, 1,044 bytes allocated

Il fatto che segnali che ci sono dei byte in uso all'uscita indica che ci siamo dimenticati di invocare *free* prima del termine del programma (cosa che andrebbe sempre fatta). Il sommario mostra che ci sono state due alloc e una free: evidente mente il sistema ha fatto una alloc e la relativa free per un bocco di 1024 byte.

Vi incoraggio a provare *valgrind* tutte le volte che un programma non funziona. Nel caso dei segmentation fault, ad esempio, noterete che valgrind fornisce come minimo la riga del codice in cui si è verificato il problema.

Valgrind è open source e disponibile per Linux, Android, e con qualche limitazione MacOSX, si veda