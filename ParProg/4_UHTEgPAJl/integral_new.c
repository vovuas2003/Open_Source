/*
Can integrate functions like sin(1/x) or cos(1/x^2).
Just edit LEFT, RIGHT, N_ZEROS, f_zero and f.
*/

//#define DEBUG 1 //print reaching i max and emergency exit
//#define NZ_PRINT 1 //print N_ZEROS

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define LEFT 0.005
#define RIGHT 4.0
#define H_INIT 2 //start amount of pieces
#define I_MAX 28 //max expected h/2 for eps

int N_ZEROS = (int)((double)(1.0f / (M_PI * LEFT * LEFT)) - 0.5f) + 1;
//the rightest zero must less than RIGHT or sub some from expression above
//number of first (right) zero is 0, possible to change in f_zero function

//analytical expression for zeros of function
double f_zero(int nn) {
    return 1 / sqrt(M_PI * ((double)nn + 0.5));
}

//function to integrate
double f(double x) {
    return cos(1 / (x * x));
}

int n; //amount of threads
double *points; //don't forget left and right, from right to left
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

double trap_int(double l, double h, int nn) {
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
    old = trap_int(l, h, nn);
    h /= 2;
    nn *= 2;
    new = trap_int(l, h, nn);
    int i;
    for(i = 0; i < I_MAX; i++) {
        if(fabs(new - old) <= eps) {
            break;
        }
        old = new;
        h /= 2;
        nn *= 2;
        new = trap_int(l, h, nn);
    }
    #ifdef DEBUG
    if(i == I_MAX) {
        printf("i max is reached, exit!\n");
        exit(-1);
    }
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
    points = (double*)malloc((N_ZEROS + 2) * sizeof(double));
    if((thid == NULL) || (args == NULL) || (points == NULL)) {
        printf("Malloc error!\n");
        exit(-1);
    }
    #ifdef NZ_PRINT
    printf("Amount of zeros = %d.\n", N_ZEROS);
    #endif
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
    free(points);
    printf("Amount of threads = %d.\n", n);
    printf("Precision = %0.50lf.\n", eps);
    printf("Integral value = %0.50lf.\n", otv);
    printf("Execution time = %lf seconds.\n", ((double) ti) / CLOCKS_PER_SEC);
    return 0;
}
