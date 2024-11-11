#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *env[]) {
	int fd0, fd1;
	sscanf(argv[1], "%d", &fd0);
	sscanf(argv[2], "%d", &fd1);

	if(close(fd0) < 0) {
		printf("Child: Can\'t close reading side of pipe\n");
		exit(-1);
	}

	int size = write(fd1, "Hello, world!", 14);

	if(size != 14) {
		printf("Child: Can\'t write all string to pipe\n");
		exit(-1);
	}

	if(close(fd1) < 0) {
		printf("Child: Can\'t close writing side of pipe\n");
		exit(-1);
	}

	return 0;
}
