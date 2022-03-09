#include <iostream>
#include "mpi.h"
using namespace std;

int main() {

	int rank, size;
	int n;
	int index = -1;
	bool found = false;
	int v[10] = { 7, 11, 20, 81, 67, 5, 38, 22, 24, 13 };
	MPI_Status status;

	//the number that need to be found
	n = 11;

	MPI_Init(NULL, NULL);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0) {

		//there are 10 elements in v[], starting with index 0
		//assume that we need 6 processes(1 master and 5 slaves)

		int k = 0;

		//each slave process needs to check 2 numbers from v[]
		for (int i = 1; i <= size - 1; i++) {
			MPI_Send(&v[k], 2, MPI_INT, i, 1, MPI_COMM_WORLD);
			k += 2;
		}

		for (int i = 1; i <= size - 1; i++) {

			MPI_Recv(&index, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);

			if (index != -1) {
				found = true;
				cout << index;
			}

		}

		if (found == false) {
			cout << "Not found";
		}

	}

	if (rank != 0) {

		MPI_Recv(&v, 2, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

		if (v[0] == n) {
			index = 2*(rank-1);
		}
		else if (v[1] == n) {
			index = 2 * (rank - 1) + 1;
		}

		MPI_Send(&index, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
		
	}

	MPI_Finalize();

	return 0;
}
