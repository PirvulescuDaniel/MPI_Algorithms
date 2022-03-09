#include <iostream>
#include <vector>
#include "mpi.h"
#include <time.h>
using namespace std;

int main() {

	int rank, size;
	MPI_Status status;
	unsigned int m;
	int nr;
	int sum = 0;
	double t0, t1;

	MPI_Init(NULL, NULL);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	t0 = MPI_Wtime(); //save the current time

	srand(rank);

	m = rand() + 100;
	for (int i = 1; i <= m; i++) {

		nr = rand() % 1000;

		//cout << nr << " "; are u sure u want to print >=100 numbers to console?

		sum += nr;

	}

	t1 = MPI_Wtime(); //save the current time after the loop

	cout << "Sum from process " << rank << ": " << sum << " and takes " << t1-t0 << "ms to compute";

	MPI_Finalize();

	return 0;
}
