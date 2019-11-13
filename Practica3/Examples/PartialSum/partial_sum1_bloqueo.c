#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

int total_sum = 0;
sem_t sem;

void * partial_sum(void * arg) {

  sem_wait(&sem);
  
  int j = 0;
  int ni=((int*)arg)[0];
  int nf=((int*)arg)[1];

  for (j = ni; j <= nf; j++)
    total_sum = total_sum + j;

  sem_post(&sem);

  pthread_exit(0);
}

int main(/*int argc, char **argv*/void) {
  pthread_t th1, th2;

  // if (argc != 2) {
  // 	printf("Error parsing arguments.\n");
  // 	return 1;
  // }

  // int n = argv[1];

  int num1[2]={  1,   4999};
  int num2[2]={5000, 10000};

  /* Create the corresponding sem */

  // Parametros semaforo
  // Primer parametro

  // 0 -> compartido entre trheads
  // 1 -> Compartido entre los procesos

  // Segundo aprametro
  // Init value => numero de proesos a entrar al semaforo antes de cerrarse


  sem_init(&sem, 0, 1);

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
