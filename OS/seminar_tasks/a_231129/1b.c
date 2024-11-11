#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
   int     *array;
   int     semid;
   int shmid;
   char    pathname[] = "1a.c";
   key_t   key;
   long    i;
   struct sembuf buf;
   buf.sem_num = 0;
   buf.sem_flg = 0;
   int new = 1;
   if((key = ftok(pathname,0)) < 0){
     printf("Can\'t generate key\n");
     exit(-1);
   }

   if((semid = semget(key, 1, 0666|IPC_CREAT|IPC_EXCL)) < 0){

      if(errno != EEXIST){
         printf("Can\'t create sem\n");
         exit(-1);
      } else {
         if((semid = semget(key, 1, 0)) < 0){
            printf("Can\'t find sem\n");
            exit(-1);
	 }
      }
   } else {
	buf.sem_op = 1;
	if(semop(semid, &buf, 1) < 0) {
		printf("Cant init sem\n");
		exit(-1);
	}
   }

    if((shmid = shmget(key, 3*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0){

      if(errno != EEXIST){
         printf("Can\'t create memory\n");
         exit(-1);
      } else {
         if((shmid = shmget(key, 1, 0)) < 0){
            printf("Can\'t find memory\n");
            exit(-1);
	 }
         new = 0;
      }
   }

   if((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)){
      printf("Can't attach shared memory\n");
      exit(-1);
   }
   buf.sem_op = -1;
   if(semop(semid, &buf, 1) < 0) {
	printf("Cant decrease sem\n");
	exit(-1);
   }
   if(new){
      array[0] =  0;
      array[1] =  1;
      array[2] =  1;
   } else {
      array[1] += 1;
      for(i=0; i<2000000000L; i++);
      array[2] += 1;
   }

   printf
      ("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n",
      array[0], array[1], array[2]);

      buf.sem_op = 1;
      if(semop(semid, &buf, 1) < 0) {
          printf("Cant increase sem\n");
          exit(-1);
      }

   if(shmdt(array) < 0){
      printf("Can't detach shared memory\n");
      exit(-1);
   }

   return 0;
}
