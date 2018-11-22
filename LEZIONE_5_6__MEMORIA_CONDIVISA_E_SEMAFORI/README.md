Lezione 6: Laboratorio del 22/11/18 (memoria condivisa e semafori)
------------------------------------------------------------------

Modificare il programma *contaprimi_file* fatto nella lezione 3 in maniera che conti i primi presenti in un file utilizzando la memoria condivisa invece delle pipe. La soluzione deve essere impostata nel seguente modo:

-   deve essere definito un array condiviso *int buffer[10]* di 10 interi e un intero condiviso *num* che dice quanti valori da testare sono attualmente presenti nel buffer (inizialmente quindi *num = 0*).

-   deve essere definito un semaforo *sem_buffer* usato per garantire che solo un processo alla volta possa accedere alle variabili *buffer* e *num*

-   il processo padre deve leggere dal file un intero e cercare di scriverlo in *buffer*: per fare questo tenta di accedere al semaforo *sem_buffer*. Una volta ottenuto l'accesso controlla *num*:

    -   se *num==10* il buffer è pieno e il processo padre non può aggiungere il valore; di conseguenza il padre esegue una *sleep* per un secondo e successivamente riprova la stessa operazione.

    -   se *num<10* il padre aggiunge il valore al buffer incrementa *num* e passa alla lettura del valore successivo da file

-   ogni processo figlio cerca di leggere un valore dal buffer: per fare questo tenta di accedere al semaforo *sem_buffer*. Una volta ottenuto l'accesso controlla *num*:

    -   se *num==0* il buffer è vuoto e non ci sono valori pronti da testare: il processo figlio esegue una *sleep* per un secondo e successivamente riprova la stessa operazione.

    -   se *num>0* c'è almeno un valore nel buffer: il figlio legge il valore *buffer[num-1]*, decrementa *num* e verifica se il valore è primo.

-   Quando il padre finisce di leggere i valori dal file deve segnalare al cosa ai figli (altrimenti questi rimarrebbero in attesa per sempre). a tale scopo può per esempio scrivere il valore *-1* nel buffer. Ogni figlio, leggendo tale valore capisce che sono finiti i numeri da testare e termina.

-   E' necessario anche un meccanismo per sommare insieme il numero dei primi ottenuto dai vari processi figli; usare un metodo a scelta basato sulla memoria condivisa/semafori.

**Nota:** questo metodo ha valore puramente didattico, quando avremo fatto altri strumenti saremo in grado di risolvere lo stesso problema in maniera più efficiente ed elegante.

### Esercizio per casa.

Si consideri l'esercizio 1 del 15/11 che effettuava il sorting di un array di *N* interi compresi tra 0 e 1999 utilizzando due processi. Si scriva una nuova versione del programma che ripsetti i seguenti vincoli

-   Non devono essere usate pipe: i dati tra i processi devono essere scambiati mediante la memoria condivisa

-   Si devono allocare 2 array di *N* interi nella memoria condivisa ma nessun altro array da parte di nessuno dei processi

-   Come nell'esercizio del 15/11 il padre deve ordinare gli elementi minori di 1000 mentre il figlio quelli maggiori o uguali a 1000. Questi ordinamenti possono essere fatti con la *qsort* ma devono svolti in contemporanea (cioè non e' ammesso far partire il figlio solo dopo che il padre ha ordinato la sua parte, o far partire l'ordinamento del padre solo dopo che il figlio è terminato).

Ultime modifiche: mercoledì, 21 novembre 2018, 18:08