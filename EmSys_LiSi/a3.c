#include <stdio.h>
#include <time.h>
#include <sys/neutrino.h>
#include <pthread.h>
#include <sched.h>

void waste_msecstest() {
	
	struct timespec before;
    struct timespec after;
	
	clock_gettime(CLOCK_REALTIME, &before);
	
	int i;
	
	for (i = 0; i < 100; i++) {
		double calculate = (i/(i-1)*i)/13;
	}
	
	clock_gettime(CLOCK_REALTIME, &after);
	
	long diff = after.tv_nsec - before.tv_nsec;
	printf("Time difference: %ld", diff);
}

void waste_msecs(unsigned int msecs) {
	
			struct timespec before;
			struct timespec after;
	
			clock_gettime(CLOCK_REALTIME, &before);
	
	int iterations = msecs;
	int i;
	
	for (i = 0; i < iterations; i++) {
		double calculate = (i/(i-1)*i)/13;
	}
	
			clock_gettime(CLOCK_REALTIME, &after);
	
			long diff = after.tv_nsec - before.tv_nsec;
			printf("Time difference: %ld", diff);
}

void * threadScheduling(void * vargp) {	
    printf("Hello from thread %d. My name is: ", st);
	
	pthread_attr_t attrib;
	struct sched_param param;
	pthread_attr_init(&attrib);
	pthread_attr_getschedparam(&attrib, &param);
	printf("My Priority is %d. ", *param);

	param->sched_priority= 1;
	pthread_attr_setschedparam(&attrib, &param);
    waste_msecs(100);
	pthread_exit((void *)pthread_self());
}

int main2() {
		waste_msecstest();
}

int main() {

	
	pthread_t threads[1];

	pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED );
	  int i;
	  //create threads for number maxthreads
	  for (i = 0; i < MAXTHREADS; ++i) {
		  int ret = pthread_create(&(threads[i]), NULL, threadScheduling, &i);
		  //error handling
		  if ( ret != 0 ){
			   printf ("pthread_create: %s\n", strerror(ret));
			   exit(-1);
		  }
		  fprintPt(threads[i]);
	  }
	  
	  for (i = 0; i < MAXTHREADS; ++i) {
		  int ret = pthread_join(threads[i], &rv);

		  //error handling
		  if ( ret != 0 ){
			   printf ("pthread_create: %s\n", strerror(ret));
			   exit(-1);
		  }
	  }
		
	return 0;
}