#include <stdlib.h>

#ifndef UTILS_H
#define UTILS_H

char* get_pathname() {
	char* result = malloc(sizeof(char) * 100);

	getcwd(result, 100);
	strcat(result, "/socket.sock");

	return (char*) result;
}

#endif
