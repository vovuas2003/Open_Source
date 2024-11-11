#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5 //amount of threads

int n = 0;
pthread_mutex_t m;

typedef struct Args_tag {
    int id;
} Args_t;

void *sum(void *args) {
    Args_t *arg = (Args_t*) args;
    int id = arg -> id;
    int res = 0;
    res = pthread_mutex_lock(&m);
    if(res != 0) {
        printf("Number %d: lock error!\n", id);
        return NULL;
    }
    n += id;
    printf("My number is %d, n = %d.\n", id, n);
    res = pthread_mutex_unlock(&m);
    if(res != 0) {
        printf("Number %d: unlock error!\n", id);
        return NULL;
    }
    return NULL;
}

int main() {
    pthread_t thid[N];
    Args_t args[N];
    int i, res;
    res = pthread_mutex_init(&m, NULL);
    if(res != 0) {
        printf("Init mutex error %d.\n", res);
        exit(-1);
    }
    for(i = 0; i < N; i++) {
        args[i].id = i + 1;
        res = pthread_create(&thid[i], (pthread_attr_t *)NULL, sum, (void*)&args[i]);
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
    res = pthread_mutex_destroy(&m);
    if(res != 0) {
        printf("Destroy mutex error %d.\n", res);
        exit(-1);
    }
    printf("End of main.\n");
    return 0;
}
