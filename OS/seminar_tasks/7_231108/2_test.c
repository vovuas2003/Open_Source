#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
   int     fd;

   size_t  size;
   char    resstring[14];
   char    name[] = "my.fifo";

	if(mknod(name, S_IFIFO | 0666, 0) < 0) {
		printf("2: Can\'t create %s, already exist, continue executing\n", name);
	}

      if((fd = open(name, O_RDONLY)) < 0){
         printf("2: Can\'t open FIFO for reading\n");
	 exit(-1);
      }

      size = read(fd, resstring, 14);

      if(size < 0){
         printf("2: Can\'t read string from FIFO\n");
         exit(-1);
      }

      printf("2: resstring = %s\n", resstring);

      if(close(fd) < 0){
        printf("2: Can\'t close FIFO\n"); exit(-1);
      }

   return 0;
}
