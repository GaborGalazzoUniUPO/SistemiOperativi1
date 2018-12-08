Lezione 2: lettura e scrittura da file binari
---------------------------------------------

### Argomenti trattati

-   Nella lezione precedente abbiamo visto come scrivere in un file binario con la funzione di libreria *fwrite*. Nella lezione di oggi abbiamo visto come realizzare la stessa cosa utilizzando la sistem call *write* e un file descriptor.

-   Abbiamo commentato vantaggi e svantaggi di usare i file descriptor e le system call invece del tipo *FILE ** e le funzioni di libreria. Il vantaggio principale è che si ha maggiore controllo sulle operazioni, lo svantaggio è che tutto è più complicato.

-   A meno di esigenze particolari meglio usare *FILE ** e le funzioni di libreria, ma sfortunatamente le pipe (che vedremo la prossima lezione) sono basate sui file descriptor.

-   Abbiamo visto il comando *od* per visualizzare il contenuto dei file binari (nella lezione scorsa)

-   Abbiamo visto come creare processi figli utilizzando la funzione *fork*

-   Abbiamo visto che se necessario il processo padre può attendere la terminazione dei figli mediante l'istruzione *wait*

-   Abbiamo visto come il padre possa recuperare gli 8 bit meno significativi dell'intero passato dai figli come parametro all'istruzione exit().

### Esercizi

1) Scrivere il programma *leggisc* che prende in input sulla linea di comando in nome di un file binario, legge gli interi in esso contenuti uno alla volta, e ne restituisce la somma. Il programma *leggisc* per aprire il file deve utilizzare la system call *open* e per leggere gli interi la system call *read* (man 2 read). Potete testare questo programma passandogli i file generati dal programma *scrivi* e ricordate che la somma degli interi tra *1* e *n* vale *n(n+1)/2*.

2) Scrivere il programma *leggisc_smart* che prende in input sulla linea di comando in nome di un file binario, legge *contemporaneamente* tutti gli interi in esso contenuti e ne restituisce la somma. Il programma *leggisc_smart* per aprire il file deve utilizzare la system call *open* e per leggere gli interi deve chiamare una sola volta la system call *read*. Ovviamente prima di tale chiamata deve allocare un buffer grande quanto il file; utilizzare la system call *lseek* per ottenere la dimensione del file.

3) Scrivere un programma *conta_primi2* per effettuare il conto dei primi compresi tra 1 e N utilizzando due processi. Il processo padre deve generare il processo figlio, contare i primi tra 1 e N/2 e attendere la terminazione del processo figlio. Il processo figlio deve contare il i primi tra N/2+1 e N e comunicare tale numero al padre mediante la funzione *exit* (stampare un warning se il numero dei primi trovati dal figlio sono più di 255 che e' il massimo valore che può essere trasmesso mediante la exit). Utilizzare la seguente funzione per verificare se un intero è primo.

```
bool primo(int n)
{
  int i;
  if(n<2) return false;
  if(n%2==0) {
    if(n==2)
      return true;
    else
      return false;
  }
  for (i=3; i*i<=n; i += 2) {
      if(n%i==0) {
          return false;
      }
  }
  return true;
}

```

Ultime modifiche: giovedì, 25 ottobre 2018, 16:49