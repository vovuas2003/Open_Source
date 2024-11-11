#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	uid_t uid = getuid();
	gid_t gid = getgid();
	pid_t pid = getpid();
	pid_t ppid = getppid();
	printf("uid = %d\ngid = %d\npid = %d\nppid = %d\n", uid, gid, pid, ppid);
	return 0;
}
