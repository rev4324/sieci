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
	unlink(SOCKET_PATH);
	exit(0);
}

int main(void) {
	signal(SIGINT, handle_SIGINT);

	char buf[80];

	int fd = socket(AF_UNIX, SOCK_STREAM, 0);
	perror("socket()");
	struct sockaddr_un addr;

	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

	int bind_res = bind(fd, (const struct sockaddr *) &addr, sizeof(addr));
	perror("bind()");

	int listen_res = listen(fd, 20);
	perror("listen()");

	while (1) {
		int data_socket = accept(fd, NULL, NULL);

		if (data_socket == -1) {
			perror("accept");
			exit(1);
		}

		int read_result = read(data_socket, buf, 80);

		if (read_result == -1) {
			perror("read()");
		}

		printf("%s\n", buf);

		long random_number = random();
		sprintf(buf, "%d", random_number);
		int write_res = write(data_socket, buf, 80);

		if (write_res == -1) {
			perror("write()");
		}
	}
	
	unlink(SOCKET_PATH);
	close(fd);

	return 0;
}
