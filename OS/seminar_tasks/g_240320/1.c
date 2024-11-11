/*
Run in separate folder:
mkdir test
cd test
cp ../1.c .
gcc 1.c
./a.out
rm ./*
cd ..
rmdir test/
*/

#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define N 150 //max expected recursion depth

int main() {
   char orig[] = "a";
   char name[5];
   char old[5];
   (void)umask(0);
   int fd;
   if((fd = open(orig, O_CREAT, 0666)) < 0){
     printf("Can\'t open file\n");
     exit(-1);
   }
   if(close(fd) < 0){
     printf("Can\'t close file\n");
     exit(-1);
   }
   int i = 0;
   sprintf(name, "%d", i);
   if(symlink(orig, name) != 0) {
      printf("Can\'t make first link\n");
      exit(-1);
   }
   for(i = 1; i < N; i++) {
      sprintf(old, "%d", i - 1);
      sprintf(name, "%d", i);
      if(symlink(old, name) != 0){
         printf("Can\'t make link\n");
         exit(-1);
      }
      if((fd = open(name, O_CREAT, 0666)) < 0){
         printf("Can\'t open link\n");
         break;
      }
      if(close(fd) < 0){
        printf("Can\'t close link\n");
        exit(-1);
      }
   }
   printf("%d\n", i);
   return 0;
}
