#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

#define N 100
#define STEPS 300
#define ALPHA 0.10

double *grid, *temp;

/* Initialize local chunk */
void initialize(int rank, int rows) {

    for(int i=0;i<rows+2;i++)
        for(int j=0;j<N;j++)
            grid[i*N+j] = temp[i*N+j] = 0.0;

    int global_start = rank * rows;

    for(int i=1;i<=rows;i++) {

        int gi = global_start + (i-1);

        /* Source 1 */
        if(gi >= 15 && gi < 25)
            for(int j=15;j<25;j++)
                grid[i*N+j] = 100.0;

        /* Source 2 */
        if(gi >= 65 && gi < 75)
            for(int j=65;j<75;j++)
                grid[i*N+j] = 100.0;

        /* Source 3 */
        if(gi >= 35 && gi < 55)
            for(int j=75;j<85;j++)
                grid[i*N+j] = 100.0;
    }
}

/* Keep sources hot every step */
void apply_sources(int rank, int rows) {

    int global_start = rank * rows;

    for(int i=1;i<=rows;i++) {

        int gi = global_start + (i-1);

        if(gi >= 15 && gi < 25)
            for(int j=15;j<25;j++)
                grid[i*N+j] = 100.0;

        if(gi >= 65 && gi < 75)
            for(int j=65;j<75;j++)
                grid[i*N+j] = 100.0;

        if(gi >= 35 && gi < 55)
            for(int j=75;j<85;j++)
                grid[i*N+j] = 100.0;
    }
}

int main(int argc,char *argv[]) {

    int rank,size;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    int rows = N / size;

    grid = (double*)malloc((rows+2) * N * sizeof(double));
    temp = (double*)malloc((rows+2) * N * sizeof(double));

    initialize(rank, rows);

    double t1 = MPI_Wtime();

    for(int step=0; step<STEPS; step++) {

        MPI_Request req[4];

        /* Exchange top halo */
        if(rank > 0) {
            MPI_Isend(&grid[1*N], N, MPI_DOUBLE, rank-1, 0,
                      MPI_COMM_WORLD, &req[0]);
            MPI_Irecv(&grid[0], N, MPI_DOUBLE, rank-1, 0,
                      MPI_COMM_WORLD, &req[1]);
        }

        /* Exchange bottom halo */
        if(rank < size-1) {
            MPI_Isend(&grid[rows*N], N, MPI_DOUBLE, rank+1, 0,
                      MPI_COMM_WORLD, &req[2]);
            MPI_Irecv(&grid[(rows+1)*N], N, MPI_DOUBLE, rank+1, 0,
                      MPI_COMM_WORLD, &req[3]);
        }

        if(rank > 0) {
            MPI_Wait(&req[0], MPI_STATUS_IGNORE);
            MPI_Wait(&req[1], MPI_STATUS_IGNORE);
        }

        if(rank < size-1) {
            MPI_Wait(&req[2], MPI_STATUS_IGNORE);
            MPI_Wait(&req[3], MPI_STATUS_IGNORE);
        }

        /* Hybrid compute */
        #pragma omp parallel for collapse(2)
        for(int i=1;i<=rows;i++) {
            for(int j=1;j<N-1;j++) {

                temp[i*N+j] =
                    grid[i*N+j] +
                    ALPHA * (
                        grid[(i-1)*N+j] +
                        grid[(i+1)*N+j] +
                        grid[i*N+j-1] +
                        grid[i*N+j+1] -
                        4.0 * grid[i*N+j]
                    );
            }
        }

        /* Swap */
        double *swap = grid;
        grid = temp;
        temp = swap;

        apply_sources(rank, rows);
    }

    /* Gather full grid */
    double *full = NULL;

    if(rank == 0)
        full = (double*)malloc(N * N * sizeof(double));

    MPI_Gather(
        &grid[1*N], rows*N, MPI_DOUBLE,
        full, rows*N, MPI_DOUBLE,
        0, MPI_COMM_WORLD
    );

    double t2 = MPI_Wtime();

    if(rank == 0) {

        FILE *fp = fopen("heatmap_hybrid.csv","w");

        for(int i=0;i<N;i++) {
            for(int j=0;j<N;j++) {
                fprintf(fp,"%e", full[i*N+j]);
                if(j < N-1) fprintf(fp,",");
            }
            fprintf(fp,"\n");
        }

        fclose(fp);

        printf("Hybrid Time: %f sec\n", t2 - t1);
        printf("Generated: heatmap_hybrid.csv\n");

        free(full);
    }

    free(grid);
    free(temp);

    MPI_Finalize();
    return 0;
}