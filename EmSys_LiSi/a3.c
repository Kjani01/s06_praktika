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
    printf("Hello from thread %d\n", pthread_self());

	pthread_attr_t attrib;
	struct sched_param param;
	pthread_attr_init(&attrib);
	//int ret = pthread_attr_getschedparam(&attrib, &param);
	int policy;
	policy = SCHED_RR;

	int ret = pthread_getschedparam(pthread_self(), &policy, &param);
	if ( ret != 0 ){
					   printf ("pthread_getschedparam: %s\n", strerror(ret));
					   exit(-1);
				  }
	//int i = param->sched_priority;
	printf("My Priority is %d. \n", param.sched_priority);

	/*param.sched_priority= 100;
	ret = pthread_attr_setschedparam(&attrib, &param);
	 if ( ret != 0 ){
				   printf ("pthread_attr_setschedparam: %s\n", strerror(ret));
				   exit(-1);
			  }*/

	ret = pthread_setschedprio(pthread_self(), 255);
	if ( ret != 0 ){
						   printf ("pthread_setschedprio: %s\n", strerror(ret));
						   exit(-1);
					  }

    waste_msecs(1);

	ret = pthread_getschedparam(pthread_self(), &policy, &param);
	if ( ret != 0 ){
					   printf ("pthread_getschedparam: %s\n", strerror(ret));
					   exit(-1);
				  }
	printf("My Priority is now %d. \n", param.sched_priority);
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
