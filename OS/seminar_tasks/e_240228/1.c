#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10000000

long i = 0;

void *mythread(void *dummy) {
   for(long j = 0; j < N; j++) {
      i++;
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
      i++;
   }
   result = pthread_join(thid, (void **)NULL);
   if(result != 0){
      printf("Error on thread join, return value = %d\n", result);
      exit(-1);
   }
   printf("i = %ld\n", i);
   return 0;
}
