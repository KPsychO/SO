#include <stdio.h> 
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include <semaphore.h> 

#define NR_PHILOSOPHERS 5

#define THINKING 2 
#define HUNGRY 1 
#define EATING 0

pthread_t philosophers[NR_PHILOSOPHERS];
pthread_mutex_t forks[NR_PHILOSOPHERS];

sem_t mutex;
sem_t S[NR_PHILOSOPHERS];
int state[NR_PHILOSOPHERS];

void init()
{
    int i;
    for(i=0; i<NR_PHILOSOPHERS; i++)
        pthread_mutex_init(&forks[i],NULL);
    
}

void take_fork(int phi){
    sem_wait(&mutex);

    printf("Philosopher %d is hungry... \n", phi);
    state[phi] = HUNGRY;

    test(phi);

    sem_post(&mutex);
    sem_wait(&S[phi]);

    sleep(1);

}

void put_fork(int phi){

    sem_wait(&mutex);

    state[phi] = THINKING;

    printf("Philosopher %d putting down fork %d and %d\n", phi, (phi+1) % NR_PHILOSOPHERS, (phi + NR_PHILOSOPHERS -1) % NR_PHILOSOPHERS);
    printf("Philosopher %d is thinking\n");

    think(phi);

    test((phi+1) % NR_PHILOSOPHERS);
    test((phi + NR_PHILOSOPHERS -1) % NR_PHILOSOPHERS);
    sem_post(&mutex);

}

void think(int i) {
    printf("Philosopher %d thinking... \n" , i);
    sleep(random() % 10);
    printf("Philosopher %d stopped thinking!!! \n" , i);

}

void eat(int i) {
    printf("Philosopher %d eating... \n" , i);
    sleep(random() % 5);
    printf("Philosopher %d is not eating anymore!!! \n" , i);

}

void toSleep(int i) {
    printf("Philosopher %d sleeping... \n" , i);
    sleep(random() % 10);
    printf("Philosopher %d is awake!!! \n" , i);
    
}

void test(int phi){

    if(state[phi] == HUNGRY
    && state[(phi+1) % NR_PHILOSOPHERS] != EATING
    && state[(phi + NR_PHILOSOPHERS -1) % NR_PHILOSOPHERS] != EATING){
        state[phi] = EATING;
        sleep(2);
        printf("Philosofer %d takes fork %d and %d\n", phi, (phi+1) % NR_PHILOSOPHERS, (phi + NR_PHILOSOPHERS -1) % NR_PHILOSOPHERS);
        eat(phi);

        sem_post(&S[phi]);
    }

}

void* philosopher(void* i)
{
    int nPhilosopher = (int)i;
    int right = nPhilosopher;
    int left = (nPhilosopher - 1 == -1) ? NR_PHILOSOPHERS - 1 : (nPhilosopher - 1);
    while(1)
    {
        
        think(nPhilosopher);
        
        take_fork(nPhilosopher);

        eat(nPhilosopher);
        
        put_fork(nPhilosopher);
        
        toSleep(nPhilosopher);
   }

}

int main()
{
    init();
    unsigned long i;

    // initialize the semaphores 
    sem_init(&mutex, 0, 1); 
  
    for (i = 0; i < NR_PHILOSOPHERS; i++) sem_init(&S[i], 0, 0); 

    for(i=0; i<NR_PHILOSOPHERS; i++) pthread_create(&philosophers[i], NULL, philosopher, (void*)i);
    
    for(i=0; i<NR_PHILOSOPHERS; i++) pthread_join(philosophers[i],NULL);
    
    return 0;
} 
