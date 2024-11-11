#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
   char     *array;
   int     shmid;
   char    pathname[] = "1.c";
   key_t   key;
	size_t size = 4;
	struct shmid_ds *buf;

   if((key = ftok(pathname,0)) < 0){
     printf("Can\'t generate key\n");
     exit(-1);
   }

         if((shmid = shmget(key, size * sizeof(char), 0)) < 0){
            printf("Can\'t find shared memory\n");
            exit(-1);
	 }

   if((array = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)){
      printf("Can\'t attach shared memory\n");
      exit(-1);
   }

	printf("%s\n", array);

   if(shmdt(array) < 0){
      printf("Can\'t detach shared memory\n");
      exit(-1);
   }
	if(shmctl(shmid, IPC_RMID, buf) < 0) {
		printf("Can\'t delete shared memory\n");
		exit(-1);
	}
   return 0;
}
