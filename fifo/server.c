#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <math.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include "./config.h"

void handle_SIGINT() {
	remove(FIFO_SERVER_PATH);
	remove(FIFO_CLIENT_PATH);
	_exit(0);
}

int main(void) {
	mkfifo(FIFO_SERVER_PATH, 0666);
	mkfifo(FIFO_CLIENT_PATH, 0666);

	signal(SIGINT, handle_SIGINT);

	int server_fd = open(FIFO_SERVER_PATH, O_RDWR);
	int client_fd = open(FIFO_CLIENT_PATH, O_RDWR);

	char buf[80];

	while (1) {
		long d = 0L;
		double x = 0.0;
		long s = random() % 1000;

		puts("Reading d...");
		read(client_fd, buf, 80);
		d = atol(buf);

		puts("Reading x...");
		read(client_fd, buf, 80);
		x = atof(buf);

		puts("Calculating z...");
		double z = pow(x, d) + x * sinl(d) + s;
		printf("z = %f^%d + %f * sin(%d) + %d\n", x, d, x, d, s);
		printf("z = %f\n", z);

		puts("Sending z...");
		sprintf(buf, "%f", z);
		write(server_fd, buf, 80);

		puts("Reading y...");
		read(client_fd, buf, 80);
		double y = atof(buf);
		printf("y = %f\n", y);
	}
	
	close(server_fd);
	close(client_fd);

	return 0;
}
