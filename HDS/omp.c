#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100
#define STEPS 200
#define ALPHA 0.10

double A[N][N], B[N][N];

/* Initialize grid with multiple heat sources */
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

/* Keep sources hot every step */
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

/* Export CSV */
void export_csv() {

    FILE *fp = fopen("omp_heatmap.csv","w");

    for(int i=0;i<N;i++) {
        for(int j=0;j<N;j++) {
            fprintf(fp,"%e", A[i][j]);
            if(j < N-1) fprintf(fp,",");
        }
        fprintf(fp,"\n");
    }

    fclose(fp);
}

int main() {

    init();

    double start = omp_get_wtime();

    for(int step=0; step<STEPS; step++) {

        #pragma omp parallel for collapse(2)
        for(int i=1;i<N-1;i++) {
            for(int j=1;j<N-1;j++) {

                B[i][j] = A[i][j] + ALPHA * (
                    A[i-1][j] +
                    A[i+1][j] +
                    A[i][j-1] +
                    A[i][j+1] -
                    4.0 * A[i][j]
                );
            }
        }

        #pragma omp parallel for collapse(2)
        for(int i=1;i<N-1;i++)
            for(int j=1;j<N-1;j++)
                A[i][j] = B[i][j];

        apply_source();
    }

    double end = omp_get_wtime();

    printf("OMP Time: %f sec\n", end - start);

    export_csv();

    printf("Generated: omp_heatmap.csv\n");

    return 0;
}