/*
Hago HE 16, a 17 pa3Mep resstring
u yBeJIu4uTb HA 1 size B read, write u npoBepKe HA < 0
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
   int     fd[2], fd_back[2], result;

   size_t size;
   char  resstring[16];

   if(pipe(fd) < 0){
     printf("Can\'t open pipe\n");
     exit(-1);
   }

   if(pipe(fd_back) < 0){
     printf("Can\'t open back pipe\n");
     exit(-1);
   }

   result = fork();

   if(result < 0) {
      printf("Can\'t fork child\n");
      exit(-1);
   } else if (result > 0) {

     /* Parent process */

      if(close(fd[0]) < 0){
         printf("parent: Can\'t close reading side of pipe\n"); exit(-1);
      }

      size = write(fd[1], "Hello, my child!", 15);

      if(size != 15){
        printf("Can\'t write all string to pipe\n");
        exit(-1);
      }

      if(close(fd[1]) < 0){
         printf("parent: Can\'t close writing side of pipe\n"); exit(-1);
      }
	//
	if(close(fd_back[1]) < 0){
         printf("parent: Can\'t close writing side of back pipe\n"); exit(-1);
      }

      size = read(fd_back[0], resstring, 16);

      if(size < 0){
         printf("Can\'t read string from back pipe\n");
         exit(-1);
      }

      printf("Parent exit, resstring:%s\n", resstring);

      if(close(fd_back[0]) < 0){
         printf("parent: Can\'t close reading side of pipe\n"); exit(-1);
      }
//

     // printf("Parent exit\n");

   } else {

      /* Child process */

      if(close(fd[1]) < 0){
         printf("child: Can\'t close writing side of pipe\n"); exit(-1);
      }

      size = read(fd[0], resstring, 15);

      if(size < 0){
         printf("Can\'t read string from pipe\n");
         exit(-1);
      }

      printf("Child exit, resstring:%s\n", resstring);

      if(close(fd[0]) < 0){
         printf("child: Can\'t close reading side of pipe\n"); exit(-1);
      }
//
	if(close(fd_back[0]) < 0){
         printf("parent: Can\'t close reading side of back pipe\n"); exit(-1);
      }

      size = write(fd_back[1], "Hello, my parent!", 16);

      if(size != 16){
        printf("Can\'t write all string to pipe\n");
        exit(-1);
      }

      if(close(fd_back[1]) < 0){
         printf("parent: Can\'t close writing side of pipe\n"); exit(-1);
      }
//

   }

   return 0;
}
