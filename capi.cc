#include <mpi.h>
#include "capi.h"
#include <iostream>

int pass_by_val(int num)
{
	std::cout << num << std::endl;
        int in = num;
        int out;
        for(int i=0; i<10; i++)
          MPI_Allreduce(&in,&out,1,MPI_INTEGER,MPI_SUM,MPI_COMM_WORLD);
	return 0;
}


