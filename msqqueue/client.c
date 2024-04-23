#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mqueue.h>
#include <signal.h>
#include <string.h>
#include <time.h>

int onError(char* str) {
	perror(str);
	return EXIT_FAILURE;
}

int main(void) {
	char buf[80];

	mqd_t mq = mq_open("/calc", O_RDWR);

	printf("MQ opened\n");

	if (mq == -1) {
		puts(strerror(errno));
	}

	puts("Enter d (long):");
	fgets(buf, 80, stdin);
	if (mq_send(mq, buf, 80, 1) == -1) {
		return onError("Sending d");
	}

	puts("Enter x (double):");
	fgets(buf, 80, stdin);
	if (mq_send(mq, buf, 80, 1) == -1) {
		return onError("Sending x");
	}

	puts("Reading z...");
	if (mq_receive(mq, buf, 80, NULL) == -1) {
		return onError("Receiving z");
	}

	double z = atof(buf);

	srand(time(NULL));
	int t = rand() % 1000;
	printf("t = %d\n", t);

	puts("Calculating y...");
	double y = z + t;
	printf("y = %f\n", y);
	sprintf(buf, "%f", y);

	puts("Sending y...");
	if (mq_send(mq, buf, 80, 1) == -1) {
		return onError("Sending y");
	}
	
	mq_close(mq);
	
	return EXIT_SUCCESS;

	// while (1) {
	// 	long d = 0L;
	// 	double x = 0.0;
	// 	long s = random() % 1000;
	//
	// 	puts("Reading d...");
	// 	read(client_fd, buf, 80);
	// 	d = atol(buf);
	//
	// 	puts("Reading x...");
	// 	read(client_fd, buf, 80);
	// 	x = atof(buf);
	//
	// 	puts("Calculating z...");
	// 	double z = pow(x, d) + x * sinl(d) + s;
	// 	printf("z = %f^%d + %f * sin(%d) + %d\n", x, d, x, d, s);
	// 	printf("z = %f\n", z);
	//
	// 	puts("Sending z...");
	// 	sprintf(buf, "%f", z);
	// 	write(server_fd, buf, 80);
	//
	// 	puts("Reading y...");
	// 	read(client_fd, buf, 80);
	// 	double y = atof(buf);
	// 	printf("y = %f\n", y);
	// }
}
