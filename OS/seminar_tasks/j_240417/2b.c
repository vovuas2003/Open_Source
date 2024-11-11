#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int f = 1, b = 0;

void my_handler1(int nsig){
	b = 0;
	f = 0;
}

void my_handler2(int nsig){
	b = 1;
	f = 0;
}

int main() {
    pid_t pid;
    int n = 0, i = 0;
    int res;
    (void)signal(SIGUSR1, my_handler1);
    (void)signal(SIGUSR2, my_handler2);
    printf("Receiver pid = %d\n", getpid());
    printf("Write sender pid: ");
    scanf("%d", &pid);
    while(i < 32) {
        while(f);
	f = 1;
	n = (n << 1) | b;
        i++;
	res = kill(pid, SIGUSR1);
        if(res < 0) {
            printf("Sending signal error!\n");
            exit(-1);
        }
    }
    printf("Received int value = %d\n", n);
    return 0;
}
