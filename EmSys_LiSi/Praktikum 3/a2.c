#include <stdio.h>
#include <time.h>
#include <sys/neutrino.h>

//192.168.8.2

/*int changeSystemTick(unsigned int microsecs) {
	//int t = ticksize;
    printf("SYSTEM CLOCK GRANULARITY %d\n", ticksize);
    ticksize (microsecs * 1000);
    printf("SYSTEM CLOCK GRANULARITY %d\n", ticksize);
    return 0;
}*/

/*void changeSystemTick(unsigned int microsecs)
{
	struct _clockperiod clockRes;
	struct _clockperiod clockRes_new;

	int error;

	//aktuelle zeit ausgeben
	error = ClockPeriod(CLOCK_REALTIME, NULL, &clockRes, 0);
	if (error != 0) {
		perror("Error in ClockPeriod");
	}
	printf("Current Clock Resolution: %lu\n", clockRes.nsec);

	// neue auflösung setzen in nsec
	clockRes_new.nsec = microsecs * 1000;
	clockRes_new.fract = 0;

	error = ClockPeriod(CLOCK_REALTIME, &clockRes_new, NULL, 0);
	if (error != 0) {
		perror("Error in ClockPeriod");
	}

	//neue auflösung ausgeben
	error = ClockPeriod(CLOCK_REALTIME, NULL, &clockRes, 0);
	if (error != 0) {
		perror("Error in ClockPeriod");
	}
	printf("New Clock Resolution: %lu\n", clockRes.nsec);
}


int main() {

	changeSystemTick(5000);
	return 0;

}*/

int main() {
    printf("Der 1ms-Sleep\n");

    struct timespec starttime;
    clock_gettime(CLOCK_REALTIME, &starttime);

    struct timespec current;
    struct timespec deadline;
    struct timespec aftertime;

    clock_gettime(CLOCK_REALTIME, &current);
    clock_gettime(CLOCK_REALTIME, &deadline);

    deadline.tv_sec += 5;

    unsigned long i = 0;

    clock_gettime(CLOCK_REALTIME, &aftertime);

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

    printf("Iterations: %lu\n", i);

    printf("       START TIME %lld.%.9ld\n", (long long)starttime.tv_sec, starttime.tv_nsec);
    printf("PLANNED STOP TIME %lld.%.9ld\n", (long long)deadline.tv_sec, deadline.tv_nsec);
    printf("   REAL STOP TIME %lld.%.9ld\n", (long long)aftertime.tv_sec, aftertime.tv_nsec);

    return 0;
}
