#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main() {

	char input = 'a';
	int led0 = 0;
	int led1 = 0;
	int led2 = 0;
	int led3 = 0;

	char by = '\0';

	//char[] path = "/dev/leds";
	int fh = open("/dev/leds", O_WRONLY);

	if ( fh == -1 ){
	   printf ("open: %s\n", strerror(fh));
	   exit(-1);
	}

	while (input != 'q') {

		if (input == '1') {
			led0 = (led0 + 1) % 2;
			by ^= 1UL << 0;
		}
		else if (input == '2') {
			led1 = (led1 + 1) % 2;
			by ^= 1UL << 1;
		}

		led2 = (led0 + led1) %2;
		led3 = min((led0 + led1), 1);

		if (led2 == 1) {
			by ^= (-1 ^ by) &(1UL<<2);
		}
		else {
			by ^= (0 ^ by) &(1UL<<2);
		}

		if (led3 == 1) {
				by ^= (-1 ^ by) &(1UL<<3);
			}
			else {
				by ^= (0 ^ by) &(1UL<<3);
			}


		if (input != '\n') {
			printf ("Current Byte: 0000 %d%d%d%d\n", led3, led2, led1, led0);
		}


		/*int i;
		for (i = 0; i < 8; i++) {
			printf("%d", !!((by<<i)&0x80));
		}
		printf("\n");
		*/

		ssize_t ret = write(fh, &by, sizeof(by));
		if ( ret == -1 ){
		   printf ("write: %s\n", strerror(fh));
		   exit(-1);
		}

		input = getc(stdin);
	}

	int ret = close(fh);
	if ( ret != 0 ){
	   printf ("close: %s\n", strerror(fh));
	   exit(-1);
	}

	return 0;
}
