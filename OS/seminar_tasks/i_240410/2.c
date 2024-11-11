#include <signal.h>
#include <stdio.h>

void my_handler(int nsig){
   if(nsig == 2){
      printf("Receive signal %d, CTRL-C pressed\n", nsig);
   } else if(nsig == 3){
      printf("Receive signal %d, CTRL-4 or CTRL-\\ pressed\n", nsig);
   } else {
      printf("Unknown signal %d\n", nsig);
   }
}

int main(void){

    (void)signal(SIGINT, my_handler);
    (void)signal(SIGQUIT, my_handler);
    while(1);
    return 0;

}
