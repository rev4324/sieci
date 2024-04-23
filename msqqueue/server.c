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

void handle_sigint(int signum) {
	puts("SIGINT received.");
	_exit(0);
}

int main(void) {
	char buf[80];

	struct mq_attr config;

	config.mq_maxmsg = 10;
	config.mq_msgsize = 80;

	mqd_t mq = mq_open("/calc", O_RDWR | O_CREAT, 0666, config);

	printf("MQ opened\n");

	if (mq == -1) {
		puts(strerror(errno));
	}

	signal(SIGINT, handle_sigint);

	// while (1) {
	// 	printf("sending...\n");
	// 	sprintf(buf, "%d", time(NULL));
	// 	int result = mq_send(mq, buf, 80, 1);
	//
	// 	if (result == -1) {
	// 		puts(strerror(errno));
	// 	}
	// 	usleep(10);
	// }

	while (1) {
		long d = 0L;
		double x = 0.0;
		long s = random() % 1000;

		puts("Reading d...");
		if (mq_receive(mq, buf, 80, NULL) == -1) {
			perror("Reading d");
		}
		d = atol(buf);

		puts("Reading x...");
		if (mq_receive(mq, buf, 80, NULL) == -1) {
			perror("Reading x");
		}
		x = atof(buf);

		puts("Calculating z...");
		double z = pow(x, d) + x * sinl(d) + s;
		printf("z = %f^%d + %f * sin(%d) + %d\n", x, d, x, d, s);
		printf("z = %f\n", z);

		puts("Sending z...");
		sprintf(buf, "%f", z);
		if (mq_send(mq, buf, 80, 1) == -1) {
			perror("Sending z");
		}

		puts("Reading y...");
		if (mq_receive(mq, buf, 80, NULL) == -1) {
			perror("Reading y");
		}
		double y = atof(buf);
		printf("y = %f\n", y);
	}
	
	fprintf(stderr, "closing...\n");
	mq_close(mq);
	
	return 0;
}
