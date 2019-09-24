#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "mpi.h"
#include <string.h>

#define SIZE 100
int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
/**
 * mpicc matriz_multiplas_linhas.c -o matriz
 * mpirun -np 5 matriz
 */

int main(int argc, char** argv){
    int master = 0;
    int meu_rank, np, origem, destino, tag=0;
    char msg[100];
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&meu_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&np);


	int i,j,k,N;
	N = SIZE;
	// Inicializa Matriz com 1
	for(i = 0 ; i < SIZE; i++){
		for(j = 0; j < SIZE; j++) {
			A[i][j] = 1;
			B[i][j] = 1;
			C[i][j] = 1;
		}
	}

   	double begin = omp_get_wtime();
    // Calcula limites para cada processo
    int linhas_por_processo = SIZE/np;
    int limite_inferior = linhas_por_processo * meu_rank;
    int limite_superior = linhas_por_processo * (meu_rank + 1);
    if(meu_rank == np-1) {
        limite_superior += SIZE%np;
    }

    for(i=limite_inferior; i < limite_superior; i++) {
        for( j = 0; j < N; j++) {
            C[i][j] = 0.0;
            for (k = 0; k < N ; k++) {
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
            }
        }
    }

    if(meu_rank != master){
        for(tag=limite_inferior; tag < limite_superior; tag++) {
            MPI_Send(C[tag],SIZE,MPI_INT,master,tag,MPI_COMM_WORLD);
        }
    } else{
        int index = 0;
        for(tag = limite_superior; tag < SIZE; tag++) {
            if(tag%linhas_por_processo == 0 && index < np-1 ) {
                index++;
            }
            MPI_Recv(C[tag], SIZE, MPI_INT, index, tag, MPI_COMM_WORLD, &status);
        }
    }

    MPI_Finalize();
    double end = omp_get_wtime();
	double time_spent = (end - begin);
    if(meu_rank == master){
        printf("Tempo total: %f secs\n", time_spent);	
    }
    return 0;
}
