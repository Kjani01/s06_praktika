#include <stdio.h>
#include <time.h>
#include <sys/neutrino.h>

/*int changeSystemTick(unsigned int microsecs) {
	//int t = ticksize;
    printf("SYSTEM CLOCK GRANULARITY %d\n", ticksize);
    ticksize (microsecs * 1000);
    printf("SYSTEM CLOCK GRANULARITY %d\n", ticksize);
    return 0;
}*/

int main() {
    //changeSystemTick(1);

    printf("Der 1ms-Sleep\n");

    struct timespec current;
    struct timespec deadline;
    struct timespec aftertime;
    clock_gettime(CLOCK_REALTIME, &current);
    long long currentN = timespec2nsec(&current);
    long long deadlineN = timespec2nsec(&current);

    //printf("CurrentN: %lld", currentN);

    //deadline = current;

    deadline = current;
    deadline.tv_sec += 1;

    long long i = 0;
    /*for (i = 0; i < 10000; i++) {
        //clock_gettime(CLOCK_MONOTONIC, &deadline);
        deadline.tv_nsec += 100000;
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &deadline, NULL);
    }*/

    printf("       START TIME %lld.%.9ld\n", (long long)current.tv_sec, current.tv_nsec);
    printf("PLANNED STOP TIME %lld.%.9ld\n", (long long)deadline.tv_sec, deadline.tv_nsec);

    //double start = current.tv_sec; // + current.tv_nsec * 10^-9;
    //printf("start: %f\n", start);
    printf("deadlineN: %lld\n", deadlineN);


    while (deadlineN >= currentN) {
    	//printf("While Schleife");
        clock_gettime(CLOCK_REALTIME, &current);
        currentN = timespec2nsec(&current);
        printf(" currentN: %lld\n", currentN);

        current.tv_nsec += 1000;
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &current, NULL);
       // printf("       START TIME %lld.%.9ld\n", (long long)current.tv_sec, current.tv_nsec);
        i++;

    }

    printf("i: %lld\n", i);
    clock_gettime(CLOCK_REALTIME, &aftertime);

    /*printf("       START TIME %lld.%.9ld\n", (long long)current.tv_sec, current.tv_nsec);
    deadline = current;
    deadline.tv_sec += 1;
    printf("PLANNED STOP TIME %lld.%.9ld\n", (long long)deadline.tv_sec, deadline.tv_nsec);*/
    printf("   REAL STOP TIME %lld.%.9ld\n", (long long)aftertime.tv_sec, aftertime.tv_nsec);

    return 0;
}
