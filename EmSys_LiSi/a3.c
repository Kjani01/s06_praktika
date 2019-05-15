#include <stdio.h>
#include <time.h>
#include <sys/neutrino.h>
#include <pthread.h>
#include <sched.h>

void waste_msecstest() {

	int i;
	for (i = 0; i < 37750; i++) {
		double calculate = i/13;
	}
}

void waste_msecs(unsigned int msecs) {

	int iterations = msecs * 37750;
	int i;

	for (i = 0; i < iterations; i++) {
		double calculate = i/13;
	}
}

void * threadScheduling(void * vargp) {
    printf("Hello from thread.\n");

	pthread_attr_t attrib;
	struct sched_param param;
	pthread_attr_init(&attrib);
	pthread_attr_getschedparam(&attrib, &param);
	int i = param->sched_priority;
	printf("My Priority is %d. \n", i);

	//param->sched_priority= 1;
	pthread_attr_setschedparam(&attrib, &param);
    waste_msecs(1);
	pthread_exit((void *)pthread_self());
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

int main() {


	pthread_t threads[1];
	pthread_attr_t attrib;

	pthread_attr_setinheritsched(&attrib,PTHREAD_EXPLICIT_SCHED );
	  int i;
	  //create threads for number maxthreads
	  for (i = 0; i < 1; ++i) {
		  int ret = pthread_create(&(threads[i]), NULL, threadScheduling, &i);
		  //error handling
		  if ( ret != 0 ){
			   printf ("pthread_create: %s\n", strerror(ret));
			   exit(-1);
		  }
	  }

	  void *rv;

	  for (i = 0; i < 1; ++i) {
		  int ret = pthread_join(threads[i], &rv);

		  //error handling
		  if ( ret != 0 ){
			   printf ("pthread_create: %s\n", strerror(ret));
			   exit(-1);
		  }
	  }

	return 0;
}
