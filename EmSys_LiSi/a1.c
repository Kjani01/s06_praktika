#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void fprintPt(pthread_t pt) {

	size_t i;

    unsigned char *ptc = (unsigned char*)(void*)(&pt);
    printf("0x");
    for (i=0; i<sizeof(pt); i++) {
        printf("%02x", (unsigned)(ptc[i]));
    }
}

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

  for (i = 0; i < MAXTHREADS; ++i) {
      int ret = pthread_create(&(threads[i]), NULL, threadHello, &i);

      if ( ret != 0 ){
           printf ("pthread_create: %s\n", strerror(ret));
           exit(-1);
      }

      printf("Creating thread %d: ", i);
      fprintPt(threads[i]);
      printf(".\n");
  }

  for (i = 0; i < MAXTHREADS; ++i) {
      int ret = pthread_join(threads[i], &rv);

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
