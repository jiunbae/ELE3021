#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_THREAD 2
#define EXEC_NUM 10

int global_count = 0;
sem_t exsem;
pthread_t thread[MAX_THREAD];

void * thread_func(void * data) {
	long ltmp = (long) data;
	int id = (int)ltmp;
	int count = 0;
	
	while (count++ < EXEC_NUM) {
		sem_wait(&exsem);
		global_count ++;
		printf("Thread # %d incremented global count(%d)\n", id, global_count);
		sleep(1);
		sem_post(&exsem);
		sleep(1);
	}
}

int main(int argc, char * argv[]) {
	int i = 0 ;
	sem_init(&exsem, 0, 1);

	for (i = 0; i < MAX_THREAD; i++) {
		pthread_create(&thread[i], NULL, thread_func, (void*)i);
	}	
	
	printf("Joining threads\n");

	for (i = 0; i < MAX_THREAD; i++) {
		pthread_join(thread[i], NULL);
	}
}
