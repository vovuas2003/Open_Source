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
    pid_t pid = getpid();

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
       printf("Can\'t reach server!\n");
       exit(-1);
    }

       
       mybuf.mtype = 1;
       mybuf.a.pid = pid;
       printf("Input float value: ");
       scanf("%lg", &(mybuf.a.n));
       len = sizeof(mybuf.a);
       
       if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
         printf("Can\'t send message to server!\n");
         exit(-1);
       }
    
    
       
       if (len = msgrcv(msqid, (struct msgbuf *) &mybuf, len, pid, 0) < 0){
         printf("Can\'t receive message from server!\n");
         exit(-1);
       }
       printf("Result: %lg\n", mybuf.a.n);
    
       

    return 0;    
}
