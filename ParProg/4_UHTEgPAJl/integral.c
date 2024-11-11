//#define DEBUG 1

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N_ZEROS 12732 //amount of zeros, calculate manually
//number of first (right) zero is 0, possible to change in f_zero function
#define LEFT 0.005
#define RIGHT 4
#define H_INIT 2 //start amount of pieces
#define I_MAX 28 //max expected h/2 for eps

double f_zero(int nn) {
    return 1 / sqrt(M_PI * ((double)nn + 0.5));
}

double f(double x) {
    return cos(1 / (x * x));
}

int n; //amount of threads
double points[N_ZEROS + 2]; //don't forget left and right, from right to left
double eps; //precision

typedef struct Args_tag {
    int id;
    double s;
} Args_t;

void *zeros(void *args) {
    Args_t *arg = (Args_t*) args;
    int rank = arg -> id;
    int i;
    for(i = rank + 1; i < N_ZEROS + 1; i += n) {
        points[i] = f_zero(i - 1);
    }
    return NULL;
}

double trap_int(double l, double r, double h, int nn) {
    double s = 0.0;
    int i;
    double prev = l;
    double curr = l + h;
    for(i = 0; i < nn; i++) {
        s += f(prev) + f(curr);
        prev = curr;
        curr += h;
    }
    return s * h / 2;
}

double one_integ(double l, double r) {
    double h = (r - l) / H_INIT;
    int nn = H_INIT;
    double old, new;
    old = trap_int(l, r, h, nn);
    h /= 2;
    nn *= 2;
    new = trap_int(l, r, h, nn);
    int i;
    for(i = 0; i < I_MAX; i++) {
        if(fabs(new - old) <= eps) {
            break;
        }
        old = new;
        h /= 2;
        nn *= 2;
        new = trap_int(l, r, h, nn);
    }
    #ifdef DEBUG
    if(i == I_MAX) printf("i max is reached\n");
    #endif
    return new;
}

void *integral(void *args) {
    Args_t *arg = (Args_t*) args;
    int rank = arg -> id;
    int i;
    double l = (points[0] - points[1]) / n;
    double su = one_integ(points[1] + rank * l, points[1] + (rank + 1) * l);
    for(i = rank + 1; i < N_ZEROS + 1; i += n) {
        su += one_integ(points[i + 1], points[i]);
    }
    arg -> s = su;
    return NULL;
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Need 2 arguments: n_threads and epsilon; exit!\n");
        exit(-1);
    }
    if(sscanf(argv[1], "%d", &n) != 1) {
        printf("Error: first argument must be int; exit!\n");
        exit(-1);
    }
    if(sscanf(argv[2], "%lf", &eps) != 1) {
        printf("Error: second argument must be float; exit!\n");
        exit(-1);
    }
    pthread_t *thid = (pthread_t*)malloc(n * sizeof(pthread_t));
    Args_t *args = (Args_t*)malloc(n * sizeof(Args_t));
    if((thid == NULL) || (args == NULL)) {
        printf("Malloc error!\n");
        exit(-1);
    }
    clock_t ti = clock();
    points[0] = RIGHT;
    points[N_ZEROS + 1] = LEFT;
    int i, res;
    for(i = 0; i < n; i++) {
        args[i].id = i;
        res = pthread_create(&thid[i], (pthread_attr_t *)NULL, zeros, (void*)&args[i]);
        if(res != 0){
            printf("Error on thread create, return value = %d\n", res);
            exit(-1);
        }
    }
    for(i = 0; i < n; i++) {
        res = pthread_join(thid[i], (void **)NULL);
        if(res != 0){
            printf("Error on thread join, return value = %d\n", res);
            exit(-1);
        }
    }
    for(i = 0; i < n; i++) {
        res = pthread_create(&thid[i], (pthread_attr_t *)NULL, integral, (void*)&args[i]);
        if(res != 0){
            printf("Error on thread create, return value = %d\n", res);
            exit(-1);
        }
    }
    for(i = 0; i < n; i++) {
        res = pthread_join(thid[i], (void **)NULL);
        if(res != 0){
            printf("Error on thread join, return value = %d\n", res);
            exit(-1);
        }
    }
    double otv = args[0].s;
    for(i = 1; i < n; i++) {
        otv += args[i].s;
    }
    ti = clock() - ti;
    free(thid);
    free(args);
    printf("Amount of threads = %d.\n", n);
    printf("Precision = %0.50lf.\n", eps);
    printf("Integral value = %0.50lf.\n", otv);
    printf("Execution time = %lf seconds.\n", ((double) ti) / CLOCKS_PER_SEC);
    return 0;
}
