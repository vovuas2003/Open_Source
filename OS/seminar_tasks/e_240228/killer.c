#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    int msqid;
    char pathname[]="server.c";
    key_t  key;
    int len;

    struct mymsgbuf
    {
       long mtype;
       struct {
          pid_t pid;
          double n;
       } a;
    } mybuf;

    key = ftok(pathname, 0);
    if ((msqid = msgget(key, 0666)) < 0){
       printf("Can\'t reach server\n");
       exit(-1);
    }

       
       mybuf.mtype = 2;
       len = sizeof(mybuf.a);
       
       if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
         printf("Can\'t send message to server!\n");
         exit(-1);
       }

    return 0;    
}
