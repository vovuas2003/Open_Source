#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    pid_t chpid = fork();
    if (chpid < 0) {
	    printf("Fork error!\n");
            exit(-1);
    } else if (chpid == 0) {
	    execl("./men.out", "./men.out", NULL);
    } else {
	    execl("./ship.out", "./ship.out", NULL);
    }
    printf("Exec error!\n");
    exit(-1);
}
