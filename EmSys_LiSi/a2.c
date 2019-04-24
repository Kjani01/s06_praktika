#include <stdio.h>
#include <time.h>

int main() {
    printf("Der 1ms-Sleep\n.");
    
    struct timespec deadline;
    clock_gettime(CLOCK_MONOTONIC, &deadline);
    
    printf("%lld.%.9ld\n", (long long)deadline.tv_sec, deadline.tv_nsec);
    
    deadline.tv_nsec += 1000;
    
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &deadline, NULL);
    
    printf("%lld.%.9ld\n", (long long)deadline.tv_sec, deadline.tv_nsec);
    
    return 0;
}
