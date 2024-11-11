#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5 //amount of threads

int size, n;

typedef struct Args_tag {
    int id;
    double s;
} Args_t;

void *sum(void *args) {
    Args_t *arg = (Args_t*) args;
    int rank = arg -> id;
    double su = 0.0;
    int i;
    if(rank != 0) {
        for(i = rank * size; i >= 1 + size * (rank - 1); i--) {
            su += (double) 1.0 / i;
        }
    } else {
        for(i = n; i >= 1 + size * (N - 1); i--) {
            su += (double) 1.0 / i;
        }
    }
    arg -> s = su;
    return NULL;
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Error: argc != 2, exit!\n");
        exit(-1);
    }
    pthread_t thid[N];
    Args_t args[N];
    int i, res;
    if(sscanf(argv[1], "%d", &n) != 1) {
        printf("Error: argv[1] must be int, exit!\n");
        exit(-1);
    }
    size = n / N;
    for(i = 0; i < N; i++) {
        args[i].id = i;
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
    double otv = args[0].s;
    for(i = N - 1; i >= 1; i--) {
        otv += args[i].s;
    }
    printf("Sum of 1/n for n from 1 to %d = %lf.\n", n, otv);
    return 0;
}
