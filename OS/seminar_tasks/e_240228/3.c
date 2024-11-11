#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10000000

long i = 0;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void *mythread(void *dummy) {
   int result;
   for(long j = 0; j < N; j++) {
      result = pthread_mutex_lock(&m);
      if(result != 0) {
         printf("Lock error = %d\n", result);
         exit(-1);
      }
      i++;
      result = pthread_mutex_unlock(&m);
      if(result != 0) {
         printf("Unlock error = %d\n", result);
         exit(-1);
      }
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
      result = pthread_mutex_lock(&m);
      if(result != 0) {
         printf("Lock error = %d\n", result);
         exit(-1);
      }
      i++;
      result = pthread_mutex_unlock(&m);
      if(result != 0) {
         printf("Unlock error = %d\n", result);
         exit(-1);
      }
   }
   result = pthread_join(thid, (void **)NULL);
   if(result != 0){
      printf("Error on thread join, return value = %d\n", result);
      exit(-1);
   }
   printf("i = %ld\n", i);
   result = pthread_mutex_destroy(&m);
   if(result != 0){
      printf("Mutex destroy error = %d\n", result);
      exit(-1);
   }
   return 0;
}
