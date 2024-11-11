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
	if(p == 0) {
		execle("sqrt.out", "sqrt.out", "2", NULL, envp);
	}
	sleep(3);
	printf("Only parent must wait 3 seconds and print this message.\n");
	return 0;
}
