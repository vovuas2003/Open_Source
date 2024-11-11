#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{

    (void) execle("/bin/cat", "/bin/cat", "2.c", NULL, envp);

    printf("Error on program start\n");
    exit(-1);

    return 0;
}
