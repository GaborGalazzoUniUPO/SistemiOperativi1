	#include "../shared/xerrori.h"
	#include <stdlib.h>


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

	// funzione di confronto tra gli interi puntati da a e b 
	int intcmp(const void *a, const void *b)
	{
	return *((int *) a) - *((int *) b);
	}

	// verifica che array a[0..n] sia ordinato 
	bool check_sort(int *a, int n)
	{
	for(int i=0; i < n-1; i++)
		if(a[i] > a[i+1]) return false;
	return true;
	}



	#define DEBUG

	#ifdef DEBUG
	#include <assert.h>
	int somma(int* array, int n){
		int somma = 0;
		for(int i = 0; i<n; i++)
		somma+=array[i];
		return somma;
	}
	#endif

	void sort_child(int* array, int n, int my_pipe[2], int usless_pipe[2]){
		//Chiudo gli accessi alle pipe che non utilizzo
		xclose(usless_pipe[0]);
		xclose(usless_pipe[1]);
		//Non leggo dalla mia pipe
		xclose(my_pipe[0]);
		#ifdef DEBUG
		int s_ori = somma(array, n);
		#endif
		qsort(array, n, sizeof(int), intcmp);
		#ifdef DEBUG
		int s_aft = somma(array, n);
		assert(s_aft==s_ori);
		assert(check_sort(array, n));
		#endif
		xwrite(my_pipe[1], array, sizeof(int)*n);
		xclose(my_pipe[1]); //finito scrittura
	}

	void semi_merge_sort(int* array, int n){

		int pipe_f1_fd[2];
		int pipe_f2_fd[2];
		xpipe(pipe_f1_fd);
		xpipe(pipe_f2_fd);

		pid_t f1 = xfork();
		if(f1==0){
			sort_child(array, n/2, pipe_f1_fd, pipe_f2_fd);
			free(array);
			exit(EXIT_SUCCESS);
		}

		pid_t f2 = xfork();
		if(f2==0){
			sort_child(array+n/2, n - (n/2), pipe_f2_fd, pipe_f1_fd);
			free(array);
			exit(EXIT_SUCCESS);
		}

		//Chiudo gli accessi in scrittura alle pipes
		xclose(pipe_f1_fd[1]);
		xclose(pipe_f2_fd[1]);

		//effettuo il merge
		int el_f1;
		int el_f2;
		ssize_t read_f1 = xread(pipe_f1_fd[0], &el_f1, sizeof(int)); //Leggo i figli
		ssize_t read_f2 = xread(pipe_f2_fd[0], &el_f2, sizeof(int)); 
		for(int i = 0; i<n; i++){
			if(read_f1 && read_f2){
				if(el_f1<el_f2)
				{
					array[i] = el_f1;
					//leggo il prossimo elemento da f1
					read_f1 = xread(pipe_f1_fd[0], &el_f1, sizeof(int)); 
				}else{
					array[i] = el_f2;
					//leggo il prossimo elemento da f2
					read_f2 = xread(pipe_f2_fd[0], &el_f2, sizeof(int)); 
				
				}
			}else if(read_f1){
				array[i] = el_f1;
				//leggo il prossimo elemento da f1
				read_f1 = xread(pipe_f1_fd[0], &el_f1, sizeof(int)); 
			}else{
				array[i] = el_f2;
				//leggo il prossimo elemento da f2
				read_f2 = xread(pipe_f2_fd[0], &el_f2, sizeof(int)); 
			}
		}
		xclose(pipe_f1_fd[0]); //Chiudo definitivamente le pipe
		xclose(pipe_f2_fd[0]);
	}

	int main(int argc, String argv[]){
		if(argc != 2){
			printf("Uso: %s num\n", argv[0]);
			exit(EXIT_FAILURE);
		}
		// Il processo padre legge il numero di elementi n 
		// dell'array dalla linea di comando e costruisce un 
		// array di n elementi random compresi tra 0 e 1999. 
		int n = atoi(argv[1]);
		int* array = random_array(n);

		#ifdef DEBUG
		int s_ori = somma(array, n);
		#endif

		semi_merge_sort(array, n);

		#ifdef DEBUG
		int s_aft = somma(array, n);
		assert(check_sort(array, n));
		assert(s_aft==s_ori);
		#endif

		free(array);
	}