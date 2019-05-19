#include <mpi.h>
#include "capi.h"
#include <cstdlib>
#include <ctime>
#include <map>
#include <vector>
#include <string>
#include <iostream>

int pass_by_val(int num)
{
	// Allreduce
	reduce(num);

	// Matrix - matrix multiplication part
	int mpi_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
	std::srand(std::time(nullptr) + mpi_rank); 
	int M = 1000;
	double** A = new double*[M];
	double** B = new double*[M];
	double** C = new double*[M];

	for(int i = 0; i < M; i++) {
		A[i] = new double[M];
		B[i] = new double[M];
		C[i] = new double[M];
	}

	for (int i=0; i<M; i++){
		for (int j=0; j<M; j++){
			A[i][j] = std::rand()/((double)RAND_MAX + 1.0);
			B[i][j] = std::rand()/((double)RAND_MAX + 1.0);
			C[i][j] = 0.0;
		}
	}

	std::cout << A[5][10] << std::endl;

	for (int i=0; i<M; i++)
		for (int j=0; j<M; j++)
			for (int k=0; k<M; k++)
				C[i][j]	= C[i][j] + A[i][k]*B[k][j];		

	std::cout << A[5][10] << std::endl;

	for(int i = 0; i < M; i++) {
		delete [] A[i];
		delete [] B[i];
		delete [] C[i];
	}
	delete [] A;
	delete [] B;
	delete [] C;

	return 0;
}

int multimap_stats()
{
	int mpi_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
	// Build a random multimap
	std::multimap<std::string,int> obj;
	int min=0, max=3, ind=-1;	
	int vmin = 100, vmax = 500;
	std::vector<std::string> keys = {"a", "b", "c"};
	for (int i=0; i<100000; i++)
		obj.insert(std::pair<std::string,int>(keys.at(std::rand() % (max - min) + min), std::rand() % (vmax - vmin + 1) + vmin)); 
	// Print its stats
	// Count occurrences of a key
  std::cout << "a: " << obj.count("a") << "\nb: " << obj.count("b")
            << "\nc: " << obj.count("c") << "\n";
 	// Count occurrences of each value
 	for (int i=vmin; i<=vmax; i++){
		int val = i, count = 0;
	  for (const auto& entry : obj)
			if (entry.second == val)
				count++;
		if ((count > 0) && !(val%100)) 
	  	std::cout << "One rank " << mpi_rank << " value " << val << " occurred " << count << " times." << std::endl;		 
	}
	return 0;
}

void reduce(int num)
{
	// Allreduce part
	std::cout << num << std::endl;
	int in = num;
	int out;
	for(int i=0; i<100; i++)
	  MPI_Allreduce(&in,&out,1,MPI_INTEGER,MPI_SUM,MPI_COMM_WORLD);
}
