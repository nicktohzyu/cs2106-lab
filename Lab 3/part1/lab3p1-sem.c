#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <pthread.h>

#define NUM_PROCESSES 5

// Compile with -lpthread
int main() {

    int shmid;
    sem_t* sems;

    int i, j, pid;

    shmid = shmget(IPC_PRIVATE, sizeof(sem_t) * 4, IPC_CREAT | 0600);
    sems = (sem_t *) shmat(shmid, NULL, 0);

    for (int k = 0; k < NUM_PROCESSES - 1; k++) 
    {
        sem_init(&sems[k], 1, 0);
    }
    
    for(i=0; i<NUM_PROCESSES; i++)
    {
        if((pid = fork()) == 0) {
            break;
        }
    }

    if(pid == 0) {
        if (i != 0) {
            sem_wait(&sems[i-1]);
        }
        printf("I am child %d\n", i);

        for(j = i*10; j<i*10 + 10; j++){
            printf("%d ", j);
            fflush(stdout);
            usleep(250000);
        }

        printf("\n\n");
        sem_post(&sems[i]);
    }
    else {
        for(i=0; i<NUM_PROCESSES; i++) 
            wait(NULL);
    }

    for (int k = 0; k < NUM_PROCESSES - 1; k++) 
    {
        sem_destroy(&sems[k]);
    }
    shmctl(shmid, IPC_RMID, 0);

}

