#include <stdio.h>
#include <time.h>
#include <sys/neutrino.h>
#include <pthread.h>
#include <sched.h>
#include <unix.h>
#include <semaphore.h>


	sem_t ourSem;

void sleeping();

void waste_msecs(unsigned int msecs) {

	long iterations = msecs * 20010;
	long i;
	volatile double calculate;
	for (i = 0; i < iterations; i++) {
		calculate = i/13*(i+1);
	}
	//printf("Ende Waste Msecs, Iterations: %ld\n", iterations);
	calculate++;
}



void * threadSetSemaphore(void * vargp) {
    //printf("Hello from Task 1.\n");

	int ret = pthread_setschedprio(pthread_self(), 50);
	if ( ret != 0 ){
	   printf ("pthread_setschedprio: %s\n", strerror(ret));
	   exit(-1);
	}

	int i = 0;

	struct timespec current;
    struct timespec deadline;

	int j;
	for (j = 0; j < 1000; j++) {

		i++;

	    clock_gettime(CLOCK_REALTIME, &current);
	    clock_gettime(CLOCK_REALTIME, &deadline);

	    //deadline.tv_nsec = 2000000;
	    //In 4 ms sind wir fertig
	    nsec2timespec(&deadline, timespec2nsec(&deadline) + 4000000);

	   // printf("Vor Waste Msecs\n");
		waste_msecs(2);


	    int errorCode = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &deadline, NULL);
		if (errorCode != 0)
		{
			printf("Error in Nanosleep: %s\n", strerror(errorCode));
			continue;
		}


		if (i == 3) {
		ret = sem_post(&ourSem);
		if ( ret != 0 ){
		   printf ("sem_post: %s\n", strerror(ret));
		   exit(-1);
		}
		//printf("> Task 1: i'm now posting\n");
		i = 0;
		}
	    //printf("Task 1 finished.\n");
	}

	pthread_exit((void *)pthread_self());
}

void * threadWaitForSemaphore(void * vargp) {
	//printf("Hello from Task 2.\n");

	int ret = pthread_setschedprio(pthread_self(), 100);
		if ( ret != 0 ){
		   printf ("pthread_setschedprio: %s\n", strerror(ret));
		   exit(-1);
		}

		int j;
	for (j = 0; j < 1000; j++) {

	    //printf("> Task 2: sem_t @ %d\n", semv);

	    //printf("> Task 2: waiting on my sem.\n");
		ret = sem_wait(&ourSem);
		if ( ret != 0 ){
				   printf ("sem_wait: %s\n", strerror(ret));
				   exit(-1);
				}
		//printf("> Task 2: sem has been posted, continuing.\n");
		//nap(2);
		//waste_msecs2(3);
		waste_msecs(3);
	    //printf("Task 2 finished.\n");
	}

	pthread_exit((void *)pthread_self());

}

int main2() {



	//Semaphore

	int ret = sem_init( &ourSem, 0, 1 );

		//error handling
		if ( ret != 0 ){
		   printf ("pthread_create: %s\n", strerror(ret));
		   exit(-1);
		}

	pthread_t threads[2];

		  ret = pthread_create(&(threads[0]), NULL, threadSetSemaphore, NULL);
		  //error handling
		  if ( ret != 0 ){
			   printf ("pthread_create: %s\n", strerror(ret));
			   exit(-1);
		  }

		  ret = pthread_create(&(threads[1]), NULL, threadWaitForSemaphore, NULL);
		  //error handling
		  if ( ret != 0 ){
			   printf ("pthread_create: %s\n", strerror(ret));
			   exit(-1);
		  }

	  void *rv;

		  ret = pthread_join(threads[0], &rv);
		  //error handling
		  if ( ret != 0 ){
			   printf ("pthread_join: %s\n", strerror(ret));
			   exit(-1);
		  }

		 ret = pthread_join(threads[1], &rv);
		  //error handling
		  if ( ret != 0 ){
			   printf ("pthread_join: %s\n", strerror(ret));
			   exit(-1);
		  }

		  return 0;
}



