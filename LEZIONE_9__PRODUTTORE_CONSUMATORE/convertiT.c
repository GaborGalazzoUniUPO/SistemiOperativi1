#include "../shared/xerrori.h"

#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>
#include <string.h>

#define Buf_size 10
#define Max_line_len 100

typedef struct
{
    char **in_buf;
    sem_t *sem_in_free_slots;
    sem_t *sem_in_used_slots;

    char **out_buf;
    sem_t *sem_out_free_slots;
    sem_t *sem_out_used_slots;

    FILE* outfile;

} args_t;

void *conversion_thread_body(void *args)
{
    args_t *cargs = (args_t *)args;
    int cindex = 0;
    int oindex = 0;
    while (true)
    {
        // LEGGO DAL BUFFER
        xsem_wait(cargs->sem_in_used_slots);
        char *s = cargs->in_buf[cindex];
        xsem_post(cargs->sem_in_free_slots);

        // SPOSTO IL CURSORE
        cindex = (cindex + 1) % Buf_size;

        // EFFETTUO LA CONVERSIONE
        int len = strlen(s);
        for (int i = 0; i < len; i++)
            s[i] = isupper(s[i]) ? tolower(s[i]) : toupper(s[i]);

        // SCRIVO SUL BUFFER OUTPUT LA STRINGA CONVERTITA
        xsem_wait(cargs->sem_out_free_slots);
        cargs->out_buf[oindex] = s;
        xsem_post(cargs->sem_out_used_slots);

        // SPOSTO IL CURSORE
        oindex = (oindex + 1) % Buf_size;

        // VERIFICO LA CONDIZIONE DI FINE LETTURA
        if (len == 0)
            return NULL;
    }
}

void *output_thread_body(void *args)
{
    args_t *oargs = (args_t *)args;
    int pindex = 0;
    while (true)
    {
        // LEGGO DAL BUFFER
        xsem_wait(oargs->sem_in_used_slots);
        char *s = oargs->in_buf[pindex];
        xsem_post(oargs->sem_in_free_slots);

        // SPOSTO IL CURSORE
        pindex = (pindex + 1) % Buf_size;

        // VERIFICO LA CONDIZIONE DI FINE LETTURA
        int len = strlen(s);
        if (len == 0)
            return NULL;

        fprintf(oargs->outfile,"%s", s);
        free(s);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "\nUso: %s infile outfile\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // BUFFER IN
    char *in_buffer[Buf_size];
    sem_t sem_in_free_slots;
    sem_t sem_in_used_slots;
    xsem_init(&sem_in_free_slots, 0, Buf_size);
    xsem_init(&sem_in_used_slots, 0, 0);

    // BUFFER OUT
    char *out_buffer[Buf_size];
    sem_t sem_out_free_slots;
    sem_t sem_out_used_slots;
    xsem_init(&sem_out_free_slots, 0, Buf_size);
    xsem_init(&sem_out_used_slots, 0, 0);

    // CONVERSION THREAD
    pthread_t cthread;
    args_t cargs;
    cargs.in_buf = in_buffer;
    cargs.sem_in_free_slots = &sem_in_free_slots;
    cargs.sem_in_used_slots = &sem_in_used_slots;
    cargs.out_buf = out_buffer;
    cargs.sem_out_free_slots = &sem_out_free_slots;
    cargs.sem_out_used_slots = &sem_out_used_slots;
    xpthread_create(&cthread, NULL, conversion_thread_body, &cargs);

    FILE *ofp = xfopen(argv[2], "w");

    // OUTPUT THREAD
    pthread_t othread;
    args_t oargs;
    oargs.in_buf = out_buffer;
    oargs.sem_in_free_slots = &sem_out_free_slots;
    oargs.sem_in_used_slots = &sem_out_used_slots;
    oargs.outfile = ofp;
    xpthread_create(&othread, NULL, output_thread_body, &oargs);

    // INIZIO LETTURA FILE
    FILE *fp = xfopen(argv[1], "r");
    ssize_t read;
    int pin_index = 0;
    do
    {
        // LEGGO DA FILE LALINEA
        char *line = NULL;
        size_t len = Max_line_len;

        read = getline(&line, &len, fp);
        if (read == EOF)
        {
            free(line);
            line = "\0"; //COMUNICAZIONE FINE LETTURA
        }

        // SCRIVI SUL BUFFER IN
        xsem_wait(&sem_in_free_slots);
        in_buffer[pin_index] = line;
        xsem_post(&sem_in_used_slots);

        // SCPOSTO IL CURSORE DEL BUFFER
        pin_index = (pin_index + 1) % Buf_size;

    } while (read != EOF);

    //ASPETTO IL FIGLI
    xpthread_join(cthread, NULL);
    xpthread_join(othread, NULL);

    xfclose(fp);
    xfclose(ofp);
    return 0;
}