#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
   int     *array; // yKa3aTeJIb Ha pa3geJl9eMyI-O naM9Tb
   int     shmid; // IPC gecKpunTop
   int     new = 1;
   char    pathname[] = "06-1a.c";
   key_t   key; // IPC KJlI-O4

   if((key = ftok(pathname,0)) < 0){ // geHepaLLu9 Ha ocHoBe uMeHu u 4ucJlA 0-255
     printf("Can\'t generate key\n"); // qpAuJl goJl}|{eH r, nanKa x (npaBa gocTyna)
     exit(-1);
   }

   if((shmid = shmget(key, 3*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0){

      if(errno != EEXIST){
         printf("Can\'t create shared memory\n");
         exit(-1);
      } else {
         if((shmid = shmget(key, 3*sizeof(int), 0)) < 0){
            printf("Can\'t find shared memory\n");
            exit(-1);
	 }
         new = 0;
      }
   }

   if((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)){
      printf("Can't attach shared memory\n");
      exit(-1);
   }

   if(new){
      array[0] =  1;
      array[1] =  0;
      array[2] =  1;
   } else {
      array[0] += 1;
      array[2] += 1;
   }

   printf
      ("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n",
      array[0], array[1], array[2]);

   if(shmdt(array) < 0){
      printf("Can't detach shared memory\n");
      exit(-1);
   }

   return 0;
}
