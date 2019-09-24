#include <omp.h>
#include <math.h>
#include <time.h>
#include <iostream>

#define SIZE 50
int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
 
int main(int argc, char *argv[]) {
    int i, j, k,N, nthreads;
    clock_t clock_timer;
    double wall_timer;

	// Inicializa Matriz com 1
	for(i = 0 ; i < SIZE; i++){
		for(j = 0; j < SIZE; j++) {
			A[i][j] = 1;
			B[i][j] = 2;
			C[i][j] = 3;
		}
	}

   #pragma omp parallel shared(A,B,C,N) private(i,j,k)
	#pragma omp for 
	for(i = 0; i < SIZE; i++){
		for( j = 0; j < N; j++) {
			C[i][j] = 0.0;
			for (k = 0; k < N ; k++) {
				C[i][j] = C[i][j] + A[i][k] * B[k][j];
			}
		}
	}
}