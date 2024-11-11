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

   if((key = ftok(pathname,0)) < 0){
     printf("Can\'t generate key\n");
     exit(-1);
   }

	FILE *fin = fopen(pathname, "r");
	fseek(fin, 0, SEEK_END);
	size_t size = ftell(fin);
	fseek(fin, 0, SEEK_SET);

   if((shmid = shmget(key, size * sizeof(char), 0666|IPC_CREAT|IPC_EXCL)) < 0){

      if(errno != EEXIST){
         printf("Can\'t create shared memory\n");
         exit(-1);
      } else {
         if((shmid = shmget(key, size * sizeof(char), 0)) < 0){
            printf("Can\'t find shared memory\n");
            exit(-1);
	 } else {
		printf("Writing to already existing shared memory\n");
	}
      }
   }

   if((array = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)){
      printf("Can\'t attach shared memory\n");
      exit(-1);
   }

	fread(array, sizeof(char), size, fin);
	fclose(fin);

   if(shmdt(array) < 0){
      printf("Can\'t detach shared memory\n");
      exit(-1);
   }

   return 0;
}
