#include <omp.h>
#include <math.h>
#include <stdio.h>

/**
* gcc matriz_open.c -o matriz_open -fopenmp
*/

#define SIZE 100
int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
 
int main(int argc, char *argv[]) {
    int i, j, k;

	// Inicializa Matriz com 1
	for(i = 0 ; i < SIZE; i++){
		for(j = 0; j < SIZE; j++) {
			A[i][j] = 1;
			B[i][j] = 2;
			C[i][j] = 3;
		}
	}

	double begin = omp_get_wtime();
	#pragma omp parallel shared(A,B,C) private(i,j,k) num_threads(4)
	#pragma omp for 
	for(i = 0; i < SIZE; i++){
		for( j = 0; j < SIZE; j++) {
			C[i][j] = 0.0;
			for (k = 0; k < SIZE ; k++) {
				C[i][j] = C[i][j] + A[i][k] * B[k][j];
			}
		}
	}
	double end = omp_get_wtime();
	double time_spent = (end - begin);
	printf("Tempo total: %f secs\n", time_spent);
}