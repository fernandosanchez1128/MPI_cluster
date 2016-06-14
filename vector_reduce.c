#include <mpi.h>
#include <stdio.h>
#include <malloc.h>
#include <stdio.h>
#include <time.h>

#define MAX 1000000

int main(int argc, char** argv) {
	    clock_t t_ini, t_fin;
	    double secs;
		t_ini = clock();
        int rank, size, i;
        int vector[MAX], *miparte, stride;
        int data;
        int dest;
        int source;
        int tag = 1;
        int acum = 0;
        int total = 0;
        MPI_Status stat;
	    for (i = 1; i <= MAX; i++)
			vector[ i - 1 ] = i;
        MPI_Init(&argc,&argv);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
			stride = MAX/(size - 1);
			//printf ("stride %d",stride);	
			
        if (rank == 0) {
			printf ("rango 0");
                for (i = 1; i < size; i++)
                {
					//printf ("before");
					if (i != (size-1)) 
					{
						
						MPI_Send(&vector[stride*(i - 1)],stride,MPI_INT, i, tag, MPI_COMM_WORLD);
					}
					else 
					{
						//printf ("22"); 
						int max_stride = MAX - (stride * (i-1)); 
						MPI_Send(&vector[stride*(i - 1)],max_stride,MPI_INT, i, tag, MPI_COMM_WORLD);
					}
                        
				}
					
        } else {
                acum = 0;
                int max_stride = MAX - (stride * (size-2)); 
                miparte = (int*)malloc(sizeof(int)*max_stride);
                MPI_Recv(miparte, stride+1, MPI_INT, 0, tag, MPI_COMM_WORLD, &stat);
                if (rank != (size-1)) 
				{	
					for (i = 0, acum = 0; i < stride; i++)
                        acum += miparte[i];
				}
				else
				{
					int max_stride = MAX - (stride * (size-2)); 
					for (i = 0, acum = 0; i < max_stride; i++)
                        acum += miparte[i];
				}
        }
        MPI_Reduce(&acum,&total,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
        if (rank == 0)
                printf("Total: %d\n", total);
        MPI_Finalize();
        t_fin = clock();
        secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
		printf("%.16g milisegundos\n", secs * 1000.0);
		return 0;
}
