#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/random.h>
#include "utils.h"

void handle_SIGINT() {
	unlink(SERVER_SOCKET_PATH);
	exit(0);
}

int main(void) {
	signal(SIGINT, handle_SIGINT);

	char buf[80];

	int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	perror("socket()");
	struct sockaddr_un addr;
	struct sockaddr_un from_addr;
	socklen_t from_addr_len = sizeof(from_addr);

	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SERVER_SOCKET_PATH, sizeof(addr.sun_path) - 1);

	int bind_res = bind(fd, (const struct sockaddr *) &addr, sizeof(addr));
	perror("bind()");

	while (1) {
		recvfrom(fd, buf, 80, 0, (struct sockaddr *) &from_addr, &from_addr_len);
		perror("recvfrom()");

		printf("%s\n", buf);

		long random_long = random();
		sprintf(buf, "%d", random_long);

		sendto(fd, buf, 80, 0, (struct sockaddr *) &from_addr, from_addr_len);
		perror("sendto");

		// if (data_socket == -1) {
		// 	perror("accept");
		// 	exit(1);
		// }
		//
		// int read_result = read(data_socket, buf, 80);
		//
		// if (read_result == -1) {
		// 	perror("read()");
		// }
		//
		// printf("%s\n", buf);
		//
		// long random_number = random();
		// sprintf(buf, "%d", random_number);
		// int write_res = write(data_socket, buf, 80);
		//
		// if (write_res == -1) {
		// 	perror("write()");
		// }
	}
	
	unlink(SERVER_SOCKET_PATH);
	close(fd);

	return 0;
}
