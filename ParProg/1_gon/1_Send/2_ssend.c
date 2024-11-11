#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int np, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    char s[10241];
    for(int i = 0; i < 10240; i++) {
        s[i] = '0';
    }
    s[10240] = '\0';
    int n = 0;
    if(rank == 0) {
        for(int i = 0; i < 10240; i++) {
            s[i] = '1';
        }
        s[10240] = '\0';
        n = 1;
        double start, end;
        printf("==================================================\n");
        printf("MPI_Ssend test\n");
        MPI_Barrier(MPI_COMM_WORLD);
        start = MPI_Wtime();
        MPI_Ssend(&n, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        end = MPI_Wtime();
        printf("Ssend int time = %lg seconds\n", end - start);
        
        MPI_Barrier(MPI_COMM_WORLD);
        start = MPI_Wtime();
        MPI_Ssend(s, 10241, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        end = MPI_Wtime();
        printf("Ssend char array time = %lg seconds\n", end - start);
    } else {
        MPI_Barrier(MPI_COMM_WORLD);
        sleep(2);
        MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Recv int, n = %d\n", n);

        MPI_Barrier(MPI_COMM_WORLD);
        sleep(2);
        MPI_Recv(s, 10241, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Recv char[10241] array, array[0] = %c\n", s[0]);
        printf("==================================================\n");
    }
    MPI_Finalize();
    return 0;
}
