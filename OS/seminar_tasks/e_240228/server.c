#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    
    int msqid;
    char pathname[]="server.c";
    key_t  key;

    struct mymsgbuf
    {
       long mtype;
       struct {
          pid_t pid;
          double n;
       } a;
    } mybuf;

    key = ftok(pathname, 0);
    
    if ((msqid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL)) < 0){
       printf("Can\'t create exlusive server!\n");
       exit(-1);
    }

    int len = sizeof(mybuf.a);
    int maxlen = sizeof(mybuf.a);
    pid_t pid;
    double n;
    while(1) {
       
       if (len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, -2, 0) < 0){
         printf("Can\'t receive message from client, shut down!\n");
         msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
         exit(-1);
       }
       if(mybuf.mtype == 2) {
         printf("Shutting down\n");
         msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
         break;
       }
       pid = mybuf.a.pid;
       n = mybuf.a.n;
       n = n * n;
       mybuf.mtype = pid;
       mybuf.a.n = n;
       len = sizeof(mybuf.a);
       if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
         printf("Can\'t send message to client, shut down!\n");
         msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
         exit(-1);
       }
    }
       

    return 0;    
}
