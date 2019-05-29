#include <stdio.h>
#include <time.h>
#include <sys/neutrino.h>
#include <pthread.h>
#include <sched.h>
#include <unix.h>
#include <semaphore.h>


	sem_t ourSem;

void waste_msecstest() {

	int i;
	for (i = 0; i < 30000; i++) {
		double calculate = i/13*(i-1);
	}
}

//int main() {
//	struct timespec before;
//	struct timespec after;
//
//	int i;
//	clock_gettime(CLOCK_REALTIME, &before);
//	for (i = 0; i < 1000; i++) {
//		waste_msecstest();
//	 }
//	clock_gettime(CLOCK_REALTIME, &after);
////	if (after.tv_sec == before.tv_sec) {
//	long long diff = (after.tv_nsec - before.tv_nsec)%1000000;
//
//	//printf("Time difference %lld\n", diff/1000);
//
////	}
////	else {
////		long long diff = after.tv_sec - before.tv_sec;	//2 Sekunden Unterschied
////		long long nanodiff = after.tv_nsec - before.tv_nsec;
////
////		diff -= 1;
////		nanodiff += 1000000;
////
////		printf("Time difference %lld.%lld\n", diff, nanodiff/1000);
////
////	}
//	printf("Time difference %lld\n", diff);
//
//	printf("Startzeit %d, %d\n", before.tv_sec, before.tv_nsec);
//	printf("Stopzeit %d, %d\n", after.tv_sec, before.tv_nsec);
//
//	return 0;
//}


void sleeping();

void waste_msecs(unsigned int msecs) {

	long iterations = msecs * 20000;
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

	int j;
	for (j = 0; j < 1000; j++) {

		i++;

		//int semv;
		//sem_getvalue(&ourSem, &semv);
	    //printf("> Task 1: sem_t @ %d\n", semv);

		waste_msecs(2);
		//nap(2);
		//sleeping();

	    struct timespec deadline;
	    //clock_gettime(CLOCK_REALTIME, &deadline);
	    deadline.tv_nsec = 2000000;

	    int errorCode = clock_nanosleep(CLOCK_REALTIME, 0, &deadline, NULL);
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
		//int semv;
		//sem_getvalue(&ourSem, &semv);
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

int main() {



	//Semaphore

	int ret = sem_init( &ourSem, 0, 1 );

		//error handling
		if ( ret != 0 ){
		   printf ("pthread_create: %s\n", strerror(ret));
		   exit(-1);
		}

	pthread_t threads[2];

	//pthread_attr_t attrib;
	//pthread_attr_setinheritsched(&attrib,PTHREAD_EXPLICIT_SCHED );

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


/*void sleeping() {

	    struct timespec current;
	    struct timespec deadline;

	    clock_gettime(CLOCK_REALTIME, &current);
	    clock_gettime(CLOCK_REALTIME, &deadline);

	    deadline.tv_nsec += 2000000;

	    unsigned long i = 0;

	    while (!(current.tv_sec >= deadline.tv_sec && current.tv_nsec >= deadline.tv_nsec)) {

			//Wir müssen noch eine Sekunde drauf rechnen!

			long sec = aftertime.tv_sec;
			long nsec = aftertime.tv_nsec + 1000000; // + 1 ms
			// 10^(-9) nsec = 1 sec -> auf sec draufrechnen
			if (nsec >= 1000000000) {
				sec += nsec / 1000000000;
				nsec %= 1000000000;
			}
			aftertime.tv_sec = sec;
			aftertime.tv_nsec = nsec;

			int errorCode = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &aftertime, NULL);
	        // printf("       START TIME %lld.%.9ld\n", (long long)current.tv_sec, current.tv_nsec);
	        if (errorCode != 0)
			{
				//printf("Error in Nanosleep: %s\n", strerror(errorCode));
				continue;
			}

			i++;
			clock_gettime(CLOCK_REALTIME, &current);
	    }
}*/

//#include <stdio.h>
//#include <time.h>
//#include <sys/neutrino.h>
//#include <pthread.h>
//#include <sched.h>
//
//void waste_msecstest() {
//
//	int i;
//	for (i = 0; i < 37750; i++) {
//		double calculate = i/13;
//	}
//}
//
//void waste_msecs(unsigned int msecs) {
//
//	int iterations = msecs * 37750;
//	int i;
//
//	for (i = 0; i < iterations; i++) {
//		double calculate = i/13;
//	}
//}
//
//void * threadScheduling(void * vargp) {
//    printf("Hello from thread %d\n", pthread_self());
//
//	pthread_attr_t attrib;
//	struct sched_param param;
//	pthread_attr_init(&attrib);
//	//int ret = pthread_attr_getschedparam(&attrib, &param);
//	int policy;
//	policy = SCHED_RR;
//
//	int ret = pthread_getschedparam(pthread_self(), &policy, &param);
//	if ( ret != 0 ){
//					   printf ("pthread_getschedparam: %s\n", strerror(ret));
//					   exit(-1);
//				  }
//	//int i = param->sched_priority;
//	printf("My Priority is %d. \n", param.sched_priority);
//
//	/*param.sched_priority= 100;
//	ret = pthread_attr_setschedparam(&attrib, &param);
//	 if ( ret != 0 ){
//				   printf ("pthread_attr_setschedparam: %s\n", strerror(ret));
//				   exit(-1);
//			  }*/
//
//	ret = pthread_setschedprio(pthread_self(), 500);
//	if ( ret != 0 ){
//						   printf ("pthread_setschedprio: %s\n", strerror(ret));
//						   exit(-1);
//					  }
//
//    waste_msecs(1);
//
//	ret = pthread_getschedparam(pthread_self(), &policy, &param);
//	if ( ret != 0 ){
//					   printf ("pthread_getschedparam: %s\n", strerror(ret));
//					   exit(-1);
//				  }
//	printf("My Priority is now %d. \n", param.sched_priority);
//	pthread_exit((void *)pthread_self());
//}
//
///*int main() {
//	struct timespec before;
//	struct timespec after;
//
//	int i;
//	//for (i = 0; i < 10; i++) {
//		clock_gettime(CLOCK_REALTIME, &before);
//		waste_msecstest();
//		clock_gettime(CLOCK_REALTIME, &after);
//		long diff = after.tv_nsec - before.tv_nsec;
//		printf("Time difference %ld\n", diff);
//	//}
//
//
//	return 0;
//}*/
//
//int main() {
//
//
//	pthread_t threads[1];
//	pthread_attr_t attrib;
//
//	pthread_attr_setinheritsched(&attrib,PTHREAD_EXPLICIT_SCHED );
//	  int i;
//	  //create threads for number maxthreads
//	  for (i = 0; i < 1; ++i) {
//		  int ret = pthread_create(&(threads[i]), NULL, threadScheduling, &i);
//		  //error handling
//		  if ( ret != 0 ){
//			   printf ("pthread_create: %s\n", strerror(ret));
//			   exit(-1);
//		  }
//	  }
//
//	  void *rv;
//
//	  for (i = 0; i < 1; ++i) {
//		  int ret = pthread_join(threads[i], &rv);
//
//		  //error handling
//		  if ( ret != 0 ){
//			   printf ("pthread_create: %s\n", strerror(ret));
//			   exit(-1);
//		  }
//	  }
//
//	return 0;
//}
