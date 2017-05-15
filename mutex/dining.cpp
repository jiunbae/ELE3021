#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <unistd.h>

#define NUMBER_OF_PHIL	5
#define EXEC_TIME 		3

#define EAT_TIME 		3
#define PUT_TIME 		1
#define GET_TIME		1

#define LEFT(x) ((x+NUMBER_OF_PHIL-1) % NUMBER_OF_PHIL)
#define RIGHT(x) ((x+1) % NUMBER_OF_PHIL)

enum PHIL_STATE {
	HUNGRY = 0,
	EATING = 1,
	THINKING = 2,
};

typedef struct philosopher {
	unsigned short num_eat;
	enum PHIL_STATE state;
	pthread_t thread;
} philosopher;

philosopher phil[NUMBER_OF_PHIL];
sem_t chopstick[NUMBER_OF_PHIL], lock;

int idlewait() {
	int sleepTimeMS = (rand() % 91 + 10);
	usleep(sleepTimeMS * 1000);
	return sleepTimeMS;
}

unsigned int tick() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * (unsigned int)1000 + tv.tv_usec / 1000;
}

void initPhil(void) {
	int i;
	for (i = 0; i < NUMBER_OF_PHIL; i++) {
		phil[i].state = THINKING;
		sem_init(&chopstick[i], 0, 0);
	}
	sem_init(&lock, 0, 1);
}

void think(int i) {
	phil[i].state = THINKING;
	printf("Philosopher %d thinking...\n", i);
}

void eat(int i) {
	phil[i].state = EATING;
	phil[i].num_eat +=1;
	printf("Philosopher %d eating...\n", i);
	usleep(EAT_TIME * 10000);
}

void hungry(int i) {
	phil[i].state = HUNGRY;
	printf("Philosopher %d hungry...\n", i);
}

void test(int i) {
	if (phil[i].state == HUNGRY &&
		  phil[LEFT(i)].state != EATING &&
		  phil[RIGHT(i)].state != EATING) {
		eat(i);
		sem_post(&chopstick[i]);
	}
}

void get_chop(int i) {
	usleep(GET_TIME * 10000);
	sem_wait(&lock);
	hungry(i);
	test(i);
	sem_post(&lock);
	sem_wait(&chopstick[i]);
}

void put_chop(int i) {
	usleep(PUT_TIME * 10000);
	sem_wait(&lock);
	think(i);
	test(LEFT(i));
	test(RIGHT(i));
	sem_post(&lock);
}

void* dining(void* arg) {
	int t = EXEC_TIME;
	unsigned short left, right;
	unsigned int start_time;
	long i = (long)arg;
	
	while (t > 0) {
		start_time = tick();

		get_chop(i);
		put_chop(i);

		t -= tick()/1000 - start_time/1000;
	}
}

int main(int argc, char * argv[]) {
	pthread_t t[NUMBER_OF_PHIL];
	unsigned short i, args[NUMBER_OF_PHIL];
	unsigned short minCount = USHRT_MAX, maxCount = 0;
	long long start = 0, end = 0;
	void* thread_result;
	srand(time(NULL));
	
	initPhil();
	start = tick();
	
	/* TODO: Thread create & destroy */
	// Thread Create
	for (i = 0; i < NUMBER_OF_PHIL; i++) {
		pthread_create(&phil[i].thread, NULL, dining, (void*)i);
	}

	for (i = 0; i < NUMBER_OF_PHIL; i++) {
		pthread_join(phil[i].thread, NULL);
	} 
	
	end = tick();
	printf("\n\n");
	for (i = 0; i < NUMBER_OF_PHIL; i++) {
		printf("Philosopher %d eating count: %d\n", i, phil[i].num_eat);
	}

	printf("Total Exec Time: %lld.%lld sec\n", (end-start)/1000, (end-start)%1000);
}
