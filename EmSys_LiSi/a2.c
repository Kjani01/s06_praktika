#include <stdio.h>
#include <time.h>

int changeSystemTick(unsigned int microsecs) {
    printf("SYSTEM CLOCK GRANULARITY %d\n", ticksize)
    ticksize (microsecs * 1000);
    printf("SYSTEM CLOCK GRANULARITY %d\n", ticksize)
    return 0;
}

int main() {
    //changeSystemTick(1);
    
    printf("Der 1ms-Sleep\n.");
    
    struct timespec current;
    struct timespec deadline;
    struct timespec aftertime;
    clock_gettime(CLOCK_MONOTONIC, &deadline);
    
    deadline = current;
    
    int i = 0;
    while (i = 0; i < 1000000; i++) {
        deadline.tv_nsec += 1000;
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &deadline, NULL);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &aftertime);
    
    printf("       START TIME %lld.%.9ld\n", (long long)current.tv_sec, current.tv_nsec);
    deadline = current;
    deadline.tv_sec += 1;
    printf("PLANNED STOP TIME %lld.%.9ld\n", (long long)deadline.tv_sec, deadline.tv_nsec);
    printf("   REAL STOP TIME  %lld.%.9ld\n", (long long)aftertime.tv_sec, aftertime.tv_nsec);
    
    return 0;
}
