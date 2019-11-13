#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <math.h>

typedef struct thread {
  pthread_t thread_id;
  int init_val;
  int fin_val;
} ThreadData;

int total_sum = 0;
sem_t sem;

void * partial_sum(void * arg) {
 
  ThreadData *data = (ThreadData * ) arg;
  int j = 0;
  int ni=data->init_val;
  int nf=data->fin_val;
  
  printf("ni = %d \n", ni);
  printf("nf = %d \n", nf);

  for (j = ni; j <= nf; j++){
  	sem_wait(&sem);
    total_sum = total_sum + j;
    sem_post(&sem);
  }

  pthread_exit(0);

}

int main(int argc, char **argv) {

  if (argc != 3) {  
  	printf("Error parsing arguments.\n");
  	return 1;
  }

  /* Create the corresponding sem */

  // Parametros semaforo
  // Primer parametro

  // 0 -> compartido entre trheads
  // 1 -> Compartido entre los procesos

  // Segundo parametro
  // Init value => numero de proesos a entrar al semaforo antes de cerrarse


  sem_init(&sem, 0, 1);

  ThreadData thread_array[atoi(argv[1])];

  int interval = floor(atoi(argv[2]) / atoi(argv[1]));

  for(int i = 0; i < atoi(argv[1]); i++){

    thread_array[i].init_val = 1 + (i*interval);
    if (atoi(argv[1]) == i +1) thread_array[i].fin_val = atoi(argv[2]);
    else thread_array[i].fin_val = interval + (i*interval);

  }


  // create the threads
  for(int i = 0; i < atoi(argv[1]); i++){
    pthread_create(&(thread_array[i].thread_id), NULL, partial_sum, (void *)(thread_array + i));
  }

  /* Two threads are created */

  // pthread_create(&th1, NULL, partial_sum, (void*)num1);
  // pthread_create(&th2, NULL, partial_sum, (void*)num2);


  /* the main thread waits until both threads complete */

  // pthread_join(th1, NULL);
  // pthread_join(th2, NULL);

  // Join the treads
  for(int i = 0; i < atoi(argv[1]); i++){
    pthread_join(thread_array[i].thread_id, NULL);
  }

  printf("total_sum = %d \n", total_sum);

  sem_destroy(&sem);

  return 0;
}
