#include <stdio.h>
#include <unistd.h>
int main() {
	int fd[2];
	if(pipe(fd) < 0) {
		printf("Error!\n");
		return 0;
	}
	int res = 0;
	while(fd[0] != fd[1]) {
		res = res + write(fd[1], "a", 1);
		printf("%d\n", res);
		//printf("%d\n", &fd[1] - &fd[0]);
	}
	//printf("%d\n", res);
	return 0;
}
