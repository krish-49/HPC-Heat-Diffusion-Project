#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 100
#define STEPS 200
#define ALPHA 0.10

double A[N][N], B[N][N];

/* Initialize multiple heat sources */
void init() {

    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            A[i][j] = B[i][j] = 0.0;

    /* Source 1 */
    for(int i=15;i<25;i++)
        for(int j=15;j<25;j++)
            A[i][j] = 100.0;

    /* Source 2 */
    for(int i=65;i<75;i++)
        for(int j=65;j<75;j++)
            A[i][j] = 100.0;

    /* Source 3 */
    for(int i=35;i<55;i++)
        for(int j=75;j<85;j++)
            A[i][j] = 100.0;
}

/* Keep sources hot every timestep */
void apply_source() {

    for(int i=15;i<25;i++)
        for(int j=15;j<25;j++)
            A[i][j] = 100.0;

    for(int i=65;i<75;i++)
        for(int j=65;j<75;j++)
            A[i][j] = 100.0;

    for(int i=35;i<55;i++)
        for(int j=75;j<85;j++)
            A[i][j] = 100.0;
}

int main(int argc,char *argv[]) {

    int rank,size;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    init();

    MPI_Bcast(A, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double t1 = MPI_Wtime();

    for(int step=0; step<STEPS; step++) {

        /* Rank 0 computes full grid */
        if(rank == 0) {

            for(int i=1;i<N-1;i++) {
                for(int j=1;j<N-1;j++) {

                    B[i][j] =
                        A[i][j] +
                        ALPHA * (
                            A[i-1][j] +
                            A[i+1][j] +
                            A[i][j-1] +
                            A[i][j+1] -
                            4.0 * A[i][j]
                        );
                }
            }

            for(int i=1;i<N-1;i++)
                for(int j=1;j<N-1;j++)
                    A[i][j] = B[i][j];

            apply_source();
        }

        /* Share updated grid */
        MPI_Bcast(A, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    double t2 = MPI_Wtime();

    if(rank == 0) {

        FILE *fp = fopen("mpi_heatmap.csv","w");

        for(int i=0;i<N;i++) {
            for(int j=0;j<N;j++) {
                fprintf(fp,"%e", A[i][j]);
                if(j < N-1) fprintf(fp,",");
            }
            fprintf(fp,"\n");
        }

        fclose(fp);

        printf("MPI Time: %f sec\n", t2 - t1);
        printf("Generated: mpi_heatmap.csv\n");
    }

    MPI_Finalize();
    return 0;
}