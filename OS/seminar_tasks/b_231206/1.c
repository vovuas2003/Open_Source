#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>

int main() {
	int n = 5;

	int fd[2], result, semid;
	size_t size;
	char res[7];
	char text[7];
	char pathname[] = "1.c";
	key_t key = ftok(pathname, 0);
	struct sembuf buf;
	buf.sem_num = 0;
	buf.sem_flg = 0;

	if(pipe(fd) < 0){
		printf("Can\'t open pipe\n");
		exit(-1);
   	}

	if((semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL)) < 0){
		printf("Can\'t create new semaphore set\n");
		exit(-1);
	}

	result = fork();

	if(result < 0) {
		printf("Can\'t fork child\n");
		exit(-1);
	} else if (result > 0) { /* Parent process */

		text[0] = 'P';
		text[1] = 'a';
		text[2] = 'r';
		text[3] = 'e';
		text[4] = 'n';
		text[5] = 't';
		text[6] = '\0';

		for(int i = 0; i < n; i++){
			size = write(fd[1], text, 7);
			if(size < 0){
				printf("Parent: write error\n");
				exit(-1);
			}
			buf.sem_op = 2;
			if(semop(semid, &buf, 1) < 0){
				printf("Parent: increase error\n");
				exit(-1);
			}
			buf.sem_op = 0;
			if(semop(semid, &buf, 1) < 0){
				printf("Parent: wait error\n");
				exit(-1);
			}
			size = read(fd[0], res, 7);
			if(size < 0){
				printf("Parent: read error\n");
				exit(-1);
			}
			printf("Parent: res = %s\n", res);
		}

		if(close(fd[1]) < 0){
			printf("Parent: Can\'t close writing side of pipe\n");
			exit(-1);
		}

		if(close(fd[0]) < 0){
			printf("Parent: Can\'t close reading side of pipe\n");
			exit(-1);
		}

	} else { /* Child process */

		text[0] = 'C';
		text[1] = 'h';
		text[2] = 'i';
		text[3] = 'l';
		text[4] = 'd';
		text[5] = '\0';
		buf.sem_op = -1;

		for(int i = 0; i < n; i++){

			if(semop(semid, &buf, 1) < 0){
				printf("Child: decrease error\n");
				exit(-1);
			}
			size = read(fd[0], res, 7);
			if(size < 0){
				printf("Child: read error\n");
				exit(-1);
			}
			size = write(fd[1], text, 7);
			if(size < 0){
				printf("Child: write error\n");
				exit(-1);
			}
			printf("Child: res = %s\n", res);
			if(semop(semid, &buf, 1) < 0){
				printf("Child: decrease error\n");
				exit(-1);
			}

		}

		if(close(fd[1]) < 0){
			printf("Child: Can\'t close writing side of pipe\n");
			exit(-1);
		}

		if(close(fd[0]) < 0){
			printf("Child: Can\'t close reading side of pipe\n");
			exit(-1);
		}

	}

	return 0;
}
