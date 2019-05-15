#include <stdio.h>
#include <time.h>
#include <sys/neutrino.h>
#include <pthread.h>
#include <sched.h>
#include <unix.h>
#include <semaphore.h>

void waste_msecstest() {

	int i;
	for (i = 0; i < 37750; i++) {
		double calculate = i/13;
	}
}

/*int main() {
	struct timespec before;
	struct timespec after;

	int i;
	//for (i = 0; i < 10; i++) {
		clock_gettime(CLOCK_REALTIME, &before);
		waste_msecstest();
		clock_gettime(CLOCK_REALTIME, &after);
		long diff = after.tv_nsec - before.tv_nsec;
		printf("Time difference %ld\n", diff);
	//}


	return 0;
}*/

void waste_msecs(unsigned int msecs) {

	int iterations = msecs * 37750;
	int i;

	for (i = 0; i < iterations; i++) {
		double calculate = i/13;
	}
}

void * threadSetSemaphore(void * vargp) {
    printf("Hello from Task 1.\n");
	sem_t ourSem = *(int *)vargp;

	/*pthread_attr_t attrib;
	struct sched_param param;
	pthread_attr_init(&attrib);
	pthread_attr_getschedparam(&attrib, &param);
	int i = param->sched_priority;
	printf("My Priority is %d. \n", i);

	//param->sched_priority= 1;
	pthread_attr_setschedparam(&attrib, &param);*/
    
	int i = 0;
	
	while (true) {
		i++;
		if (i == 3) {
			sem_post(&ourSem);
			i = 0;
		}
		waste_msecs(1);
		nap(2);
	}
	
	pthread_exit((void *)pthread_self());
}

void * threadWaitForSemaphore(void * vargp) {
	printf("Hello from Task 2.\n");
	sem_t ourSem = *(int *)vargp;
	
	while (true) {
		sem_wait(&ourSem);
		waste_msecs(3);
	}
	
	pthread_exit((void *)pthread_self());
	
}

int main() {
	
	//Semaphore
	
	sem_t ourSem;
	int ret = sem_init( ourSem, 1, unsigned value );
	
		//error handling
		if ( ret != 0 ){
		   printf ("pthread_create: %s\n", strerror(ret));
		   exit(-1);
		}
	
	pthread_t threads[2];
	//pthread_attr_t attrib;

	//pthread_attr_setinheritsched(&attrib,PTHREAD_EXPLICIT_SCHED );
	  //create threads for number maxthreads
		  ret = pthread_create(&(threads[0]), NULL, threadSetSemaphore, &ourSem);
		  //error handling
		  if ( ret != 0 ){
			   printf ("pthread_create: %s\n", strerror(ret));
			   exit(-1);
		  }
		  
		  ret = pthread_create(&(threads[1]), NULL, threadWaitForSemaphore, &ourSem);
		  //error handling
		  if ( ret != 0 ){
			   printf ("pthread_create: %s\n", strerror(ret));
			   exit(-1);
		  }
	  
	  void *rv;

		  ret = pthread_join(threads[0], &rv);
		  //error handling
		  if ( ret != 0 ){
			   printf ("pthread_create: %s\n", strerror(ret));
			   exit(-1);
		  }
		  
		 ret = pthread_join(threads[1], &rv);
		  //error handling
		  if ( ret != 0 ){
			   printf ("pthread_create: %s\n", strerror(ret));
			   exit(-1);
		  }
}