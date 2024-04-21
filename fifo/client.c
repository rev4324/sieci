#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>

#include "./config.h"

int main(void) {
	int client_fd = open(FIFO_CLIENT_PATH, O_RDWR);
	int server_fd = open(FIFO_SERVER_PATH, O_RDWR);

	if (client_fd < 0) {
		perror(FIFO_CLIENT_PATH);
		puts("Looks like the server isn't running...");
		return EXIT_FAILURE;
	}

	if (server_fd < 0) {
		perror(FIFO_SERVER_PATH);
		puts("Looks like the server isn't running...");
		return EXIT_FAILURE;
	}

	char buf[80];

	puts("Enter d (long):");
	fgets(buf, 80, stdin);
	write(client_fd, buf, 80);

	puts("Enter x (double):");
	fgets(buf, 80, stdin);
	write(client_fd, buf, 80);

	puts("Reading z...");
	read(server_fd, buf, 80);
	double z = atof(buf);
	int t = random() % 1000;
	printf("t = %d\n", t);

	puts("Calculating y...");
	double y = z + t;
	printf("y = %f\n", y);
	sprintf(buf, "%f", y);

	puts("Sending y...");
	write(client_fd, buf, 80);
	
	close(client_fd);
	close(server_fd);
	
	return EXIT_SUCCESS;
}
