#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//Only one thread can do Z(s[i])
//because when s[i] == 0 only one signal is sending
#define K 3 //trips
#define N 5 //men per trip
#define S 2 //amount of semaphores

int s[S] = {0, 0}; //initial semaphore values
pthread_mutex_t m[S];
pthread_cond_t cv_ad[S];
pthread_cond_t cv_z[S];
int init_resources();
int destroy_resources();

int A(int i, int n); //A(s[i], n);
int D(int i, int n); //D(s[i], n);
int Z(int i); //Z(s[i]);

typedef struct Args_tag {
    int id;
} Args_t;

void *man(void *args) {
    Args_t *arg = (Args_t*) args;
    int id = arg->id;
    int res;
    res = D(0, 1);
    if(res != 0) {
        printf("Man thread: Error value = %d\n", res);
        exit(-1);
    }
    printf("Man %d is on the ship\n", id);
    res = D(1, 1);
    if(res != 0) {
        printf("Man thread: Error value = %d\n", res);
        exit(-1);
    }
    printf("Man %d out of the ship\n", id);
    return NULL;
}

int main() {
    int time = 3, print_delay = 1, res, i;
    res = init_resources();
    if(res != 0) {
        printf("Init error %d\n", res);
        exit(-1);
    }
    pthread_t thid[N * K];
    Args_t args[N * K];
    for(i = 0; i < N * K; i++) {
        args[i].id = i + 1;
        res = pthread_create(&thid[i], (pthread_attr_t *)NULL, man, (void*)&args[i]);
        if(res != 0){
            printf("Error on thread create, return value = %d\n", res);
            exit(-1);
        }
    }
    for(int i = 1; i <= K; i++) {
        printf("Open trip number %d\n", i);
        res = A(0, N);
        if(res != 0) {
            printf("Ship thread: Error value = %d\n", res);
            exit(-1);
        }
        res = Z(0);
        if(res != 0) {
            printf("Ship thread: Error value = %d\n", res);
            exit(-1);
        }
        sleep(print_delay);
        printf("Start trip, length = %d seconds\n", time);
        sleep(time);
        printf("Finish trip\n");
        res = A(1, N);
        if(res != 0) {
            printf("Ship thread: Error value = %d\n", res);
            exit(-1);
        }
        res = Z(1);
        if(res != 0) {
            printf("Ship thread: Error value = %d\n", res);
            exit(-1);
        }
        sleep(print_delay);
    }
    for(i = 0; i < N * K; i++){
        res = pthread_join(thid[i], (void **)NULL);
        if(res != 0){
            printf("Error on thread join, return value = %d\n", res);
            exit(-1);
        }
    }
    res = destroy_resources();
    if(res != 0) {
        printf("Destroy error %d\n", res);
        exit(-1);
    }
    printf("Finish all trips\n");
    return 0;
}

int A(int i, int n) {
    int res = 0;
    res = pthread_mutex_lock(&m[i]);
    if(res != 0){
        printf("Error lock m%d\n", i);
        return res;
    }
    s[i] += n;
    for(int j = 0; j < n; j++){
        res = pthread_cond_signal(&cv_ad[i]);
        if(res != 0){
            printf("Error signal to cv_ad%d\n", i);
            return res;
        }
    }
    res = pthread_mutex_unlock(&m[i]);
    if(res != 0){
        printf("Error unlock m%d\n", i);
        return res;
    }
    return res;
}

int D(int i, int n) {
    int res = 0;
    res = pthread_mutex_lock(&m[i]);
    if(res != 0){
        printf("Error lock m%d\n", i);
        return res;
    }
    while(s[i] < n) {
        res = pthread_cond_wait(&cv_ad[i], &m[i]);
        if (res != 0){
            printf("Error wait cv_ad%d\n", i);
            return res;
        }
    }
    s[i] -= n;
    if(s[i] == 0){
        res = pthread_cond_signal(&cv_z[i]);
        if(res != 0){
            printf("Error signal to cv_z%d\n", i);
            return res;
        }
    }
    res = pthread_mutex_unlock(&m[i]);
    if(res != 0){
        printf("Error unlock m%d\n", i);
        return res;
    }
    return res;
}

int Z(int i) {
    int res = 0;
    res = pthread_mutex_lock(&m[i]);
    if(res != 0){
        printf("Error lock m%d\n", i);
        return res;
    }
    while(s[i] != 0) {
        res = pthread_cond_wait(&cv_z[i], &m[i]);
        if (res != 0){
            printf("Error wait cv_z%d\n", i);
            return res;
        }
    }
    res = pthread_mutex_unlock(&m[i]);
    if(res != 0){
        printf("Error unlock m%d\n", i);
        return res;
    }
    return res;
}

int init_resources() {
    int res = 0;
    for(int i = 0; i < S; i++) {
        res = pthread_mutex_init(&m[i], NULL);
        if(res != 0) return res;
        res = pthread_cond_init(&cv_ad[i], NULL);
        if(res != 0) return res;
        res = pthread_cond_init(&cv_z[i], NULL);
        if(res != 0) return res;
    }
    return res;
}

int destroy_resources() {
    int res = 0;
    for(int i = 0; i < S; i++) {
        res = pthread_mutex_destroy(&m[i]);
        if(res != 0) return res;
        res = pthread_cond_destroy(&cv_ad[i]);
        if(res != 0) return res;
        res = pthread_cond_destroy(&cv_z[i]);
        if(res != 0) return res;
    }
    return res;
}
