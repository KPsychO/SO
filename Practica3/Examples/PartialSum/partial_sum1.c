#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <math.h>

typedef struct thread {
  pthread thread_id;
  int init_val;
  int fin_val;
} ThreadData;

int total_sum = 0;
sem_t sem;

void * partial_sum(void * arg) {
  
  int j = 0;
  int ni=((int*)arg)[0];
  int nf=((int*)arg)[1];

  for (j = ni; j <= nf; j++){
  	sem_wait(&sem);
    total_sum = total_sum + j;
    sem_post(&sem);
  }

  pthread_exit(0);

}

int main(int argc, char **argv) {
  pthread_t th1, th2;

  if (argc != 3) {  
  	printf("Error parsing arguments.\n");
  	return 1;
  }

  /* Create the corresponding sem */

  // Parametros semaforo
  // Primer parametro

  // 0 -> compartido entre trheads
  // 1 -> Compartido entre los procesos

  // Segundo aprametro
  // Init value => numero de proesos a entrar al semaforo antes de cerrarse


  sem_init(&sem, 0, 1);

  ThreadData thread_array[atoi(argv[1])];

  int interval = floor(argv[2] / argv[1]);

  for(int i = 0; i < argv[1]; i++){

    thread_array[i].init_val = 1 + (i*interval);
    thread_array[i].fin_val = interval + (i*interval);

  }


  for(int i = 0; i < argv[1]; i++){
    // create the threads
      // THE NUMBERS MUST EB PASSES AS
        // threads_array + i
        // cant pass by index
  }

  /* Two threads are created */

  pthread_create(&th1, NULL, partial_sum, (void*)num1);
  pthread_create(&th2, NULL, partial_sum, (void*)num2);


  /* the main thread waits until both threads complete */

  pthread_join(th1, NULL);
  pthread_join(th2, NULL);

  printf("total_sum=%d and it should be 50005000\n", total_sum);

  sem_destroy(&sem);

  return 0;
}
