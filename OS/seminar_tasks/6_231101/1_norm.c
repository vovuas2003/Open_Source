#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
	int fd[2];
	if(pipe(fd) < 0) {
		printf("Error!\n");
		return 0;
	}
	if(fcntl(fd[1], F_SETFL, O_NONBLOCK) < 0) {
		printf("Can\'t fcntl\n");
		return -1;
	}
	int res = 0;
	int t = 0;
	while(1) {
		t = write(fd[1], "a", 1);
		if(t == -1) { // JIy4LLLe t < 0
			break;
		}
		res = res + t;
	}
	printf("%d\n", res);
	return 0;
}
