#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5 //amount of threads

typedef struct Args_tag {
    int id;
} Args_t;

void *hello(void *args) {
    Args_t *arg = (Args_t*) args;
    int id = arg -> id;
    printf("Hello, world! My number is %d.\n", id);
    return NULL;
}

int main() {
    pthread_t thid[N];
    Args_t args[N];
    int i, res;
    for(i = 0; i < N; i++) {
        args[i].id = i + 1;
        res = pthread_create(&thid[i], (pthread_attr_t *)NULL, hello, (void*)&args[i]);
        if(res != 0){
            printf("Error on thread create, return value = %d\n", res);
            exit(-1);
        }
    }
    for(i = 0; i < N; i++) {
        res = pthread_join(thid[i], (void **)NULL);
        if(res != 0){
            printf("Error on thread join, return value = %d\n", res);
            exit(-1);
        }
    }
    printf("Wait all threads, end of main.\n");
    return 0;
}
