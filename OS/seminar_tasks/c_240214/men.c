#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main() {
    int N = 3 * 5;
    
    char path[] = "ship.c";
    key_t key;
    if((key = ftok(path, 0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }
    
    int semid;
    if((semid = semget(key, 2, 0666|IPC_CREAT|IPC_EXCL)) < 0){
        if(errno != EEXIST){
            printf("Can\'t create sem\n");
            exit(-1);
        } else {
            if((semid = semget(key, 2, 0)) < 0){
                printf("Can\'t find sem\n");
                exit(-1);
            }
        }
    }
    struct sembuf buf;
    buf.sem_flg = 0;
    buf.sem_op = -1;
    
    pid_t chpid;
    for(int i = 1; i <= N; i++) {
        chpid = fork();
        if(chpid < 0) {
            printf("Fork error!\n");
            exit(-1);
        }
        if(chpid > 0) {
            continue;
        }
        
        buf.sem_num = 0;
        if(semop(semid, &buf, 1) < 0) {
            printf("Can\'t wait go in\n");
            exit(-1);
        }
        printf("Man number %d is on the ship\n", i);
        buf.sem_num = 1;
        if(semop(semid, &buf, 1) < 0) {
            printf("Can\'t wait go out\n");
            exit(-1);
        }
        printf("Man number %d is outside the ship\n", i);
        
        return 0;
    }
    //printf("All men is generated\n");
    return 0;
}
