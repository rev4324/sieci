#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include "utils.h"

int main(void) {
	char buf[80];
	getcwd(buf, 80);

	int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	perror("socket()");

	struct sockaddr_un addr_server;
	struct sockaddr_un addr_client;
	addr_server.sun_family = AF_UNIX;
	addr_client.sun_family = AF_UNIX;
	strncpy(addr_server.sun_path, SERVER_SOCKET_PATH, sizeof(addr_server.sun_path) - 1);
	strncpy(addr_client.sun_path, CLIENT_SOCKET_PATH, sizeof(addr_client.sun_path) - 1);

	int connect_res = connect(fd, (const struct sockaddr *) &addr_server, sizeof(addr_server));
	perror("connect()");
	if (connect_res == -1) {
		close(fd);
		exit(1);
	}

	int bind_res = bind(fd, (const struct sockaddr *) &addr_client, sizeof(addr_client));
	perror("bind()");
	if (bind_res == -1) {
		close(fd);
		exit(1);
	}
	
	write(fd, buf, 80);
	perror("write()");

	recv(fd, buf, 80, 0);
	perror("recv");

	printf("%s\n", buf);

	// int read_res = recv(fd, buf, 80);
	//
	// if (read_res == -1) {
	// 	perror("read()");
	// }

	// printf("%s\n", buf);

	unlink(CLIENT_SOCKET_PATH);
	close(fd);

	return 0;
}
