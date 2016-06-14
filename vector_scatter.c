#include "mpi.h"
#include <stdio.h>
#include <stdio.h>
#include <time.h>

double enteroCercano(double x){
    if (x - (int)x >= 0.5)
        return ceil(x);
    return floor(x);
}

main(int argc, char *argv[])  {
		clock_t t_ini, t_fin;
	    double secs;
		t_ini = clock();
        int numtasks, rank, source=0, dest, tag=1, i;
        int leng_vector = 1000000;
        float vector[leng_vector];
		for (i = 1; i <= leng_vector; i++)
			vector[ i - 1 ] = i;
        
        MPI_Status stat;
        MPI_Datatype rowtype;
		
        MPI_Init(&argc,&argv);	
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
        
        int disp[numtasks], scounts[numtasks];
        int size = enteroCercano (leng_vector / numtasks;)
        float b[size*2], acum;
        int max_size;
        for (i = 0; i < numtasks; i++) {
			if (i != (numtasks -1))
			{
                disp[i] = i*size;
                scounts[i] = size;			}
			else 
			{
				disp[i] = size * i;
				max_size = leng_vector - (size * i);
				scounts[i] = max_size;
			}
        }
        

        MPI_Scatterv(vector,scounts,disp,MPI_FLOAT,b,max_size,MPI_FLOAT,0,MPI_COMM_WORLD);
        acum = 0.0;
        if (rank==(numtasks - 1))
        {
			for (i = 0; i < max_size; i++)
                acum += b[i];
		}
		else 
		{
			for (i = 0; i < size; i++)
                acum += b[i];
		}
                
        printf("Subtotal %lf en nodo %d\n", acum, rank);
        MPI_Reduce(&acum,b,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
        if (rank == 0) printf("TOTAL: %lf\n",b[0]);
        MPI_Finalize();
        t_fin = clock();
        secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
		printf("%.16g milisegundos\n", secs * 1000.0);
		return 0;
}
