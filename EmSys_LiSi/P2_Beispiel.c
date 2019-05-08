#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/neutrino.h>

void changeSystemTick(unsigned int microsecs)
{
	struct _clockperiod clockResolution;
	struct _clockperiod clockResolution_new;

	int error;

	//aktuelle zeit ausgeben
	error = ClockPeriod(CLOCK_REALTIME, NULL, &clockResolution, 0);
	if (error != 0) {
		perror("Error in ClockPeriod");
	}
	printf("Current Clock Resolution: %lu\n", clockResolution.nsec);

	// neue auflösung setzen in nsec
	clockResolution_new.nsec = microsecs * 1000;
	clockResolution_new.fract = 0;

	error = ClockPeriod(CLOCK_REALTIME, &clockResolution_new, NULL, 0);
	if (error != 0) {
		perror("Error in ClockPeriod");
	}

	//neue auflösung ausgeben
	error = ClockPeriod(CLOCK_REALTIME, NULL, &clockResolution, 0);
	if (error != 0) {
		perror("Error in ClockPeriod");
	}
	printf("New Clock Resolution: %lu\n", clockResolution.nsec);
}

int main(int argc, char*argv[]) 
{
	struct timespec actualTime;
	struct timespec endTime;
	struct timespec nextStep;
	
	int errorCode;

	unsigned long counter = 0;
	
	clock_gettime(CLOCK_REALTIME, &endTime);
	endTime.tv_sec += 5; // 5 sec laufen lassen
	
	clock_gettime(CLOCK_REALTIME, &nextStep);
	clock_gettime(CLOCK_REALTIME, &actualTime);
	
	// genau 5sec lang laufen
	while(!(actualTime.tv_sec >= endTime.tv_sec && actualTime.tv_nsec >= endTime.tv_nsec))
	{
		// vorherige Schleifen-Werte
		long sec = nextStep.tv_sec;
		long nsec = nextStep.tv_nsec + 1000000; // + 1 ms
		// 10^(-9) nsec = 1 sec -> auf sec draufrechnen
		if (nsec >= 1000000000) {
			sec += nsec / 1000000000;
			nsec %= 1000000000;
		}
		nextStep.tv_sec = sec;
		nextStep.tv_nsec = nsec;

		errorCode = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &nextStep, NULL); //absolute time
		// 0 if request time has elapsed
		if (errorCode != 0)
		{
			printf("Nanosleep failed with error: %s\n", strerror(errorCode));
			continue;
		}

		counter++;

		clock_gettime(CLOCK_REALTIME, &actualTime);
	}

	printf("Ran %lu times\n", counter);	
}