#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main() {
    int k = 3, n = 5;
    int time = 3, delay = 1;
    
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

    for(int i = 1; i <= k; i++) {
        printf("Open trip number %d\n", i);
        buf.sem_num = 0;
        buf.sem_op = n;
        if(semop(semid, &buf, 1) < 0) {
            printf("Can\'t open ship in\n");
            exit(-1);
        }
        buf.sem_op = 0;
        if(semop(semid, &buf, 1) < 0) {
            printf("Can\'t wait all men in\n");
            exit(-1);
        }
        sleep(delay);
        printf("Start trip, length = %d seconds\n", time);
        sleep(time);
        printf("Finish trip\n");
        buf.sem_num = 1;
        buf.sem_op = n;
        if(semop(semid, &buf, 1) < 0) {
            printf("Can\'t open ship out\n");
            exit(-1);
        }
        buf.sem_op = 0;
        if(semop(semid, &buf, 1) < 0) {
            printf("Can\'t wait all men out\n");
            exit(-1);
        }
        sleep(delay);
        printf("Close trip number %d\n", i);
    }
    
    printf("Finish all trips\n");
    return 0;
}
