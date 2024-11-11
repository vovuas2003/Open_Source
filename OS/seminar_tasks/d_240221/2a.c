#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    
    int msqid;
    char pathname[]="2a.c";
    key_t  key;
    int i,len;
    int maxlen = 81;

    struct mymsgbuf
    {
       long mtype;
       struct {
          char c;
          double d;
       } a;
    } mybuf;



    /* Create or attach message queue  */
    
    key = ftok(pathname, 0);
    
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }


    /* Send information */
    
    for (i = 1; i <= 5; i++){
       
       mybuf.mtype = 1;
       mybuf.a.c = 'A';
       mybuf.a.d = 10.10;
       len = sizeof(mybuf.a);
       
       if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
         printf("Can\'t send message to queue\n");
         msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
         exit(-1);
       }
    }
    
    for (i = 1; i <= 5; i++){
       
       if (len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 2, 0) < 0){
         printf("Can\'t receive message from queue\n");
         //msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
         exit(-1);
       }
       printf("a: char = %c, double = %lf\n", mybuf.a.c, mybuf.a.d);
    }
       

    return 0;    
}
