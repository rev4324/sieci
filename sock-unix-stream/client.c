#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include "utils.h"

int main(void) {
	char buf[80];
	getcwd(buf, 80);

	int fd = socket(AF_UNIX, SOCK_STREAM, 0);
	perror("socket()");
	struct sockaddr_un addr;

	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

	int connect_res = connect(fd, (const struct sockaddr *) &addr, sizeof(addr));
	perror("connect()");
	if (connect_res == -1) {
		close(fd);
		exit(1);
	}
	
	write(fd, buf, 80);

	int read_res = read(fd, buf, 80);

	if (read_res == -1) {
		perror("read()");
	}

	printf("%s\n", buf);

	close(fd);

	return 0;
}
