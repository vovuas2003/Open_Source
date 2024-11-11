#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define A 1.0d

double next(double t, double h, double u_m1_k, double u_m_k, double f_m_k) {
    return u_m_k + t * (f_m_k - A * (u_m_k - u_m1_k) / h);
    //return u_m_k + t * f_m_k - A * t * (u_m_k - u_m1_k) / h;
}

double f(double t, double x) {
    return t + x;
}

double phi(double x) {
    return cos(M_PI * x);
}

double psi(double t) {
    return exp(-t);
}

void myprint(double* arr, int n) {
    int i;
    for(i = 0; i < n - 1; i++) {
        printf("%lf ", arr[i]);
    }
    printf("%lf\n", arr[n - 1]);
}

int main(int argc, char *argv[]) {
    clock_t ti;
    double t, h;
    int nt, nh;
    if(argc != 5) {
        printf("argc != 5\n");
        exit(-1);
    }
    if((sscanf(argv[1], "%lf", &t) * sscanf(argv[2], "%d", &nt) * sscanf(argv[3], "%lf", &h) * sscanf(argv[4], "%d", &nh)) == 0) {
        printf("Incorrect input!\n");
        exit(-1);
    }
    printf("t=%lf nt=%d h=%lf nh=%d np=1\n", t, nt, h, nh);
    double* new = (double*)malloc(nh * sizeof(double));
    double* old = (double*)malloc(nh * sizeof(double));
    double* tmp;
    if((new == NULL) || (old == NULL)) {
        printf("Malloc error!\n");
        exit(-1);
    }
    int i, j;
    ti = clock();
    for(j = nh - 1; j >= 0; j--) {
        old[j] = phi(j * h);
    }
    for(i = 1; i < nt; i++) {
        for(j = nh - 1; j > 0; j--) {
            new[j] = next(t, h, old[j - 1], old[j], f((i - 1) * t, j * h));
        }
        new[0] = psi(i * t);
        myprint(old, nh);
        tmp = new;
        new = old;
        old = tmp;
    }
    myprint(old, nh);
    ti = clock() - ti;
    free(new);
    free(old);
    printf("Execution time = %lf seconds.\n", ((double) ti) / CLOCKS_PER_SEC);
    return 0;
}
