#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int f = 0;

void my_handler1(int nsig){
	f = 0;
}

int main() {
    pid_t pid;
    int i, n, res;
    (void)signal(SIGUSR1, my_handler1);
    printf("Sender pid = %d\n", getpid());
    printf("Write receiver pid: ");
    scanf("%d", &pid);
    printf("Don\'t forget to write sender pid to receiver before input value below!\n");
    printf("Send int value: ");
    scanf("%d", &n);
    int mask = 1 << 31;
    while(mask != 0) {
        i = mask & n;
        f = 1;
        if(i) {
		res = kill(pid, SIGUSR2);
	} else {
		res = kill(pid, SIGUSR1);
	}
        if(res < 0) {
            printf("Sending signal error!\n");
            exit(-1);
        }
        mask = (mask >> 1) & 0x7fffffff;
	while(f);
    }
    printf("Sending is finished!\n");
    return 0;
}
