#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int np, rank, n;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    if(rank == 0) {
        n = 0;
        printf("Start. My rank = %d, init n = %d.\n", rank, n);
        MPI_Send(&n, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&n, 1, MPI_INT, np - 1, 0, MPI_COMM_WORLD, &status);
        printf("Finish. My rank = %d, result n = %d.\n", rank, n);
    } else {
        MPI_Recv(&n, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
        n += rank;
        printf("My rank = %d, n + rank = %d.\n", rank, n);
        if(rank == np - 1) {
            MPI_Send(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        } else {
            MPI_Send(&n, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
    return 0;
}
