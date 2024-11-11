#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10000000

long i = 0;

int ready[2] = {0, 0};
int turn;

void *mythread(void *dummy) {
   for(long j = 0; j < N; j++) {
      ready[1] = 1;
      turn = 0;
      while(ready[0] && turn == 0);
      i++;
      ready[1] = 0;
   }
   return NULL;
}

int main() {
   pthread_t thid; 
   int result;
   result = pthread_create(&thid, (pthread_attr_t *)NULL, mythread, NULL);
   if(result != 0){
      printf("Error on thread create, return value = %d\n", result);
      exit(-1);
   }
   for(long j = 0; j < N; j++) {
      ready[0] = 1;
      turn = 1;
      while(ready[1] && turn == 1);
      i++;
      ready[0] = 0;
   }
   result = pthread_join(thid, (void **)NULL);
   if(result != 0){
      printf("Error on thread join, return value = %d\n", result);
      exit(-1);
   }
   printf("i = %ld\n", i);
   return 0;
}
