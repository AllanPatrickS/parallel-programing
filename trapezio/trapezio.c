#include <stdio.h>
#include <omp.h>
#include "mpi.h"
/**
 * mpicc trapezio.c -o trapezio 
 * mpirun -np 4 trapezio
*/

#define N 20
#define THREADS 4
double x0 = 1, xn = 3;

double f(double x) {
    return x*x + 10;
}

double area_trapezio(double a, double b, double h) {
    return ((a+b) * h)/2;
}

int main(int argc, char** argv){
    int master = 0;
    int meu_rank, np, origem, destino, tag=0;
    char msg[100];
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&meu_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&np);
    omp_set_num_threads(THREADS);


	int i;
    double h = (xn - x0) / N, area_graf;

    int NperP = N/np;
    int startingT = NperP * meu_rank, endingT = NperP * (meu_rank+1);
    if(meu_rank == np-1) {
        endingT += N%np;
    }
    double v[endingT - startingT];
    // printf("rank :%d => %d %d\n", meu_rank, NperP, startingT);
    #pragma omp parallel shared(x0,xn,h) private(i,v, NperP, startingT, endingT)
	for( i = startingT; i < endingT; i++) {
        v[i-startingT] = area_trapezio(f(x0 + i*h), f(x0 + (i+1)*h), h);
        // printf("Area [%.1f , %.1f] => %.3f\n", x0 + i*h, x0 + (i+1)*h, v);
	}

    double local;
    for( i = startingT; i < endingT; i++) {
        local += v[i-startingT];        
	}
    MPI_Reduce(&local, &area_graf, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if(meu_rank == master){
        printf("-------------- Soma áreas dos trapézios --------------\n");
        printf("Integral de f no intervalo [%.1f , %.1f] => %.3f\n", x0, xn, area_graf);
    }
	
    MPI_Finalize();
    return 0;
}
