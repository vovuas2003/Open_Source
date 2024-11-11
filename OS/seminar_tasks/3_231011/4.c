#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[]) {
	pid_t p = fork();
	if(p < 0) {
		printf("Fork error, program finished!\n");
		return 0;
	}
	pid_t id = getpid();
	if(p == 0) {
		printf("\nI am a child, my pid = %d\n", id);
		execle("./sqrt_nice.out", "./sqrt_nice.out", argv[1], NULL, envp);
	}
	sleep(3);
	printf("\nI am a parent.\nOnly I must wait 3 seconds and print this message.\n");
	printf("Pid of my child = %d\n\n", p);
	return 0;
}
