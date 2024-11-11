#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	int fd[2], result;
	size_t size;
	char  resstring[14];

	if(pipe(fd) < 0){
		printf("Can\'t open pipe\n");
		exit(-1);
   	}

	result = fork();

	if(result < 0) {
		printf("Can\'t fork child\n");
		exit(-1);
	} else if (result > 0) { /* Parent process */

		if(close(fd[1]) < 0){
			printf("Parent: Can\'t close writing side of pipe\n");
			exit(-1);
		}

		size = read(fd[0], resstring, 14);

		if(size < 0){
			printf("Can\'t read string from pipe\n");
        		exit(-1);
		}

		if(close(fd[0]) < 0){
			printf("Parent: Can\'t close reading side of pipe\n");
			exit(-1);
		}

		printf("%s\n", resstring);

	} else { /* Child process */
		char fd0[100];
		char fd1[100];
		sprintf(fd0, "%d", fd[0]);
		sprintf(fd1, "%d", fd[1]);

		execl("./2_2.out", "./2_2.out", fd0, fd1, NULL);

		printf("Exec error\n");
		exit(-1);
	}

	return 0;
}
