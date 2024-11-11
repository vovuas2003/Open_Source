#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

void myprint(double* arr, int n, int t, int rank, FILE* fout) {
    int i;
    fprintf(fout, "%d:%d:", t, rank);
    for(i = 0; i < n - 1; i++) {
        fprintf(fout, "%lf ", arr[i]);
    }
    fprintf(fout, "%lf\n", arr[n - 1]);
}

int main(int argc, char *argv[]) {
    int np, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Request r;
    double t, h;
    int nt, nh;
    char name[10];
    sprintf(name, "%d.txt", rank);
    if(argc != 5) {
        printf("argc != 5\n");
        exit(-1);
    }
    if((sscanf(argv[1], "%lf", &t) * sscanf(argv[2], "%d", &nt) * sscanf(argv[3], "%lf", &h) * sscanf(argv[4], "%d", &nh)) == 0) {
        printf("Incorrect input!\n");
        exit(-1);
    }
    double n;
    double* new = (double*)malloc(nh * sizeof(double));
    double* old = (double*)malloc(nh * sizeof(double));
    double* tmp;
    if((new == NULL) || (old == NULL)) {
        printf("Malloc error!\n");
        exit(-1);
    }
    int i, j;
    FILE* fout = fopen(name, "w");
    if(rank == 0) {
        fprintf(fout, "t=%lf nt=%d h=%lf nh=%d np=%d\n", t, nt, h, nh * np, np);
        double start, end;
        start = MPI_Wtime();
        for(j = nh - 1; j >= 0; j--) {
            old[j] = phi(j * h);
        }
        for(i = 1; i < nt; i++) {
            for(j = nh - 1; j > 0; j--) {
                new[j] = next(t, h, old[j - 1], old[j], f((i - 1) * t, j * h));
            }
            new[0] = psi(i * t);
            myprint(old, nh, i, rank, fout);
            n = old[nh - 1];
            MPI_Isend(&n, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &r);
            tmp = new;
            new = old;
            old = tmp;
        }
        myprint(old, nh, nt, rank, fout);
        MPI_Recv(&n, 1, MPI_DOUBLE, np - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        end = MPI_Wtime();
        fprintf(fout, "Execution time = %lf seconds.\n", end - start);
    } else if(rank == np - 1) {
        for(j = nh - 1; j >= 0; j--) {
            old[j] = phi((j + rank * nh) * h);
        }
        for(i = 1; i < nt; i++) {
            for(j = nh - 1; j > 0; j--) {
                new[j] = next(t, h, old[j - 1], old[j], f((i - 1) * t, (j + rank * nh) * h));
            }
            MPI_Recv(&n, 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            new[0] = next(t, h, n, old[0], f((i - 1) * t, rank * nh * h));
            myprint(old, nh, i, rank, fout);
            tmp = new;
            new = old;
            old = tmp;
        }
        myprint(old, nh, nt, rank, fout);
        MPI_Isend(&n, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &r);
    } else {
        for(j = nh - 1; j >= 0; j--) {
            old[j] = phi((j + rank * nh) * h);
        }
        for(i = 1; i < nt; i++) {
            for(j = nh - 1; j > 0; j--) {
                new[j] = next(t, h, old[j - 1], old[j], f((i - 1) * t, (j + rank * nh) * h));
            }
            MPI_Recv(&n, 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            new[0] = next(t, h, n, old[0], f((i - 1) * t, rank * nh * h));
            myprint(old, nh, i, rank, fout);
            n = old[nh - 1];
            MPI_Isend(&n, 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &r);
            tmp = new;
            new = old;
            old = tmp;
        }
        myprint(old, nh, nt, rank, fout);
    }
    fclose(fout);
    free(new);
    free(old);
    MPI_Finalize();
    return 0;
}
