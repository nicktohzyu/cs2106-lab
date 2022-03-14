#include <stdio.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>

int* count;
int* numproc;
sem_t* barrier;
sem_t* mutex;
int count_shmid, numproc_shmid, barrier_shmid, mutex_shmid;

void init_barrier(int _numproc) {
    count_shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    numproc_shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    barrier_shmid = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0600);
    mutex_shmid = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0600);

	count = (int *) shmat(count_shmid, NULL, 0);
    numproc = (int *) shmat(numproc_shmid, NULL, 0);
	barrier = (sem_t *) shmat(barrier_shmid, NULL, 0);
	mutex = (sem_t *) shmat(mutex_shmid, NULL, 0);

	*count = 0;
	*numproc = _numproc;
	sem_init(barrier, 1, 0);
	sem_init(mutex, 1, 1);
}

void reach_barrier() {
	sem_wait(mutex);
    (*count)++;
	sem_post(mutex);
	if (*count == *numproc) {
		sem_post(barrier);
	} else {
		sem_wait(barrier);
		sem_post(barrier);
	}

}

void destroy_barrier(int my_pid) {
    if(my_pid != 0) {
    	sem_destroy(barrier);
    	sem_destroy(mutex);
    	shmctl(count_shmid, IPC_RMID, 0);
    	shmctl(barrier_shmid, IPC_RMID, 0);
    	shmctl(mutex_shmid, IPC_RMID, 0);
    }
}


