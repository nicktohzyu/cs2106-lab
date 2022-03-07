#include <stdio.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>

int* count;
sem_t* barrier;
sem_t* mutex;
int shmid, shmid2, shmid3;

void init_barrier(int numproc) {
	shmid = shmget(IPC_PRIVATE, sizeof(int)*2, IPC_CREAT | 0600);
	shmid2 = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0600);
	shmid3 = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT | 0600);
	count = (int *) shmat(shmid, NULL, 0);
	barrier = (sem_t *) shmat(shmid2, NULL, 0);
	mutex = (sem_t *) shmat(shmid3, NULL, 0);
	count[0] = 0;
	count[1] = numproc;
	sem_init(barrier, 1, 0);
	sem_init(mutex, 1, 1);
}

void reach_barrier() {
	sem_wait(mutex);
	count[0]++;
	sem_post(mutex);
	if (count[0] == count[1]) {
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
    	shmctl(shmid, IPC_RMID, 0);
    	shmctl(shmid2, IPC_RMID, 0);
    	shmctl(shmid3, IPC_RMID, 0);
    }
}


