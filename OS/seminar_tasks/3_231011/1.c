#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	pid_t chpid = fork();
	if (chpid < 0) {
		printf("Error!\n");
	} else if (chpid == 0) {
		printf("I am a child.\n");
	} else {
		printf("I am a parent of %d.\n", chpid);
	}
	return 0;
}
