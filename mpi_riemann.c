


#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include "mpi.h"

#define MAXRECT 100
//#define MAXTHREAD 10
#define BASE 0.0
#define LIMIT 20.0

double length = LIMIT - BASE;
double base_length = 0;


double function(double x) {
        return x * x;
}

int main(int argc, char** argv) {
        int numtasks, rank, source=0, dest, tag=1;

        base_length = length/MAXRECT;
        int i;
        MPI_Status stat;
        MPI_Datatype rowtype;
	
        MPI_Init(&argc,&argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
        int taskids[numtasks];
        float limits [numtasks];
        int sizes [numtasks];
        double numRectxThread = (double)(MAXRECT/numtasks);
        float b[1];
        for ( i= 0; i < numtasks; i++) {
				sizes[i]=1;
                taskids[i] = i;
                limits[i]= i*base_length*numRectxThread;
                //printf ("limit %f",limits[i] );
                //pthread_create(&threads[i], NULL, calcular, (void*)taskids[i]);
        }
        
        MPI_Scatterv(limits,sizes,taskids,MPI_FLOAT,b,1,MPI_FLOAT,0,MPI_COMM_WORLD);	
        double lowlimit = b[0];
        printf ("low %f - hola %f ",lowlimit,b[1]);
        float sum = 0;
		int a; 	
        for (a = 0; a < numRectxThread; a++) {
                sum += function(lowlimit + a*base_length) * base_length;
        }
        printf("Subtotal %f en nodo %d \n", sum, rank);
        MPI_Reduce(&sum,b,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
        if (rank == 0) printf("TOTAL: %lf\n",b[0]);
        MPI_Finalize();
}
