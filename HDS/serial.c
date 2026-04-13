#include <stdio.h>
#include <time.h>

#define N 100
#define STEPS 100
#define ALPHA 0.1

double A[N][N], B[N][N];

/* Apply multiple heat sources */
void apply_sources() {

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

int main() {

    /* Initialize grid */
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            A[i][j] = B[i][j] = 0.0;

    apply_sources();

    for(int t=0;t<STEPS;t++) {

        for(int i=1;i<N-1;i++) {
            for(int j=1;j<N-1;j++) {

                B[i][j] = A[i][j] + ALPHA * (
                    A[i+1][j] +
                    A[i-1][j] +
                    A[i][j+1] +
                    A[i][j-1] -
                    4.0 * A[i][j]
                );
            }
        }

        for(int i=1;i<N-1;i++)
            for(int j=1;j<N-1;j++)
                A[i][j] = B[i][j];

        /* Keep sources hot each timestep */
        apply_sources();
    }

    double mn = A[0][0], mx = A[0][0];

    for(int i=0;i<N;i++) {
        for(int j=0;j<N;j++) {
            if(A[i][j] < mn) mn = A[i][j];
            if(A[i][j] > mx) mx = A[i][j];
        }
    }

    printf("MIN = %.12f\n", mn);
    printf("MAX = %.12f\n", mx);

    FILE *fp = fopen("serial_heatmap.csv","w");

    for(int i=0;i<N;i++) {
        for(int j=0;j<N;j++) {
            fprintf(fp,"%e", A[i][j]);
            if(j < N-1) fprintf(fp,",");
        }
        fprintf(fp,"\n");
    }

    fclose(fp);

    return 0;
}