#import <stdio.h>
#import <pthread.h>
#import <unistd.h>

void * threadHello(void * vargp) {
  int st = *(int *)vargp;
  sleep(st);
  printf("Hello from thread %d.\n", st);
  pthread_exit(pthread_self);
}

void fprintPt(pthread_t pt) {
    unsigned char *ptc = (unsigned char*)(void*)(&pt);
    printf("0x");
    for (size_t i=0; i<sizeof(pt); i++) {
        printf("%02x", (unsigned)(ptc[i]));
    }
}

int main() {
  int MAXTHREADS = 2;
  printf("Creating threads ...\n");
  pthread_t t0;
  void *rv;

  pthread_t threads[MAXTHREADS];

  for (int i = 0; i < MAXTHREADS; ++i) {
      printf("Creating thread %d: ", i);
      fprintPt(threads[i]);
      printf(".\n");
      pthread_create(&(threads[i]), NULL, threadHello, &i);
  }
  
  for (int i = 0; i < MAXTHREADS; ++i) {
      pthread_join(threads[i], &rv);
      pthread_t tmp = (pthread_t)rv;
      
      printf("Joining thread %d: ", i);
      fprintPt(threads[i]);
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
