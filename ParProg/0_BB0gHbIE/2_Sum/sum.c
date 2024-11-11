#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int np, rank, n;
    sscanf(argv[1], "%d", &n);
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int size = n / np;
    double sum = 0.0;
    if(rank != 0) {
        for(int i = rank * size; i >= 1 + size * (rank - 1); i--) {
            sum += (double) 1.0 / i;
        }
        MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        double res = 0.0;
        for(int i = n; i >= 1 + size * (np - 1); i--) {
            res += (double) 1.0 / i;
        }
        MPI_Status status;
        for(int i = np - 1; i >= 1; i--) {
            MPI_Recv(&sum, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
            res += sum;
        }
        printf("Sum of 1/n for n from 1 to %d = %lf.\n", n, res);
    }
    MPI_Finalize();
    return 0;
}
