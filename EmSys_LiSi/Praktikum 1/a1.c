#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

//print function for hex addresses (copied from stackoverflow)
void fprintPt(pthread_t pt) {
	size_t i;
    unsigned char *ptc = (unsigned char*)(void*)(&pt);
    printf("0x");
    for (i=0; i<sizeof(pt); i++) {
        printf("%02x", (unsigned)(ptc[i]));
    }
}

//function creates a new thread
void * threadHello(void * vargp) {
    int st = *(int *)vargp;
    sleep(st);
    printf("Hello from thread %d. My name is: ", st);
    fprintPt(pthread_self());
    printf("\n");
    pthread_exit((void *)pthread_self());
}

int main() {
  int MAXTHREADS = 2;
    /*printf("MAIN THREAD: ");
    fprintPt(pthread_self());
    printf("\n");*/
  printf("Creating threads ...\n");
  void *rv;

  pthread_t threads[MAXTHREADS];

  int i;

  //create threads for number maxthreads
  for (i = 0; i < MAXTHREADS; ++i) {
      int ret = pthread_create(&(threads[i]), NULL, threadHello, &i);

      //error handling
      if ( ret != 0 ){
           printf ("pthread_create: %s\n", strerror(ret));
           exit(-1);
      }

      printf("Creating thread %d: ", i);
      fprintPt(threads[i]);
      printf(".\n");
  }

  //wait for thread to exit and compares addresses
  for (i = 0; i < MAXTHREADS; ++i) {
      int ret = pthread_join(threads[i], &rv);

      //error handling
      if ( ret != 0 ){
           printf ("pthread_create: %s\n", strerror(ret));
           exit(-1);
      }

      pthread_t tmp = (pthread_t) rv;

      printf("Joining thread %d: we called it ", i);
      fprintPt(threads[i]);
      printf(", it called itself ");
      fprintPt(tmp);
      printf(".\n");

      //comparison of identities
      if (pthread_equal(tmp, threads[i])) {
          printf("Thread %d had the same ID.\n", i);
      }
      else {
          printf("Thread %d had a different ID.\n", i);
      }
  }

  printf("... all threads joined.\n");

  return 0;
}
