#include <iostream>
#include <cmath>
#include "mpi.h"
using namespace std;

bool isPrime(int a) {

	if (a < 2) {
		return false;
	}

	for (int i = 2; i <= sqrt(a); i++) {
		if (a % i == 0) {
			return false;
		}
	}
	return true;
}

int main() {

	int rank, size;
	int n;
	int v[52];
	MPI_Status status;

	n = 20;

	MPI_Init(NULL, NULL);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0) {

		//assume that there are 50 numbers in v[] and we have 6 processes(1 master and 5 slaves)
		for (int i = 1; i <= 50; i++) {
			v[i] = i;
		}

		int k = 1;

		//each slave process needs to check 10 numbers from v[]
		for (int i = 1; i <= size - 1; i++) {
			MPI_Send(&v[k], 10, MPI_INT, i, 1, MPI_COMM_WORLD);
			k += 10;
		}

	}

	if (rank != 0) {

		MPI_Recv(&v, 10, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

		for (int i = 0; i <= 9; i++) {

			if (isPrime(v[i]) && v[i] <= n) {
				cout << v[i] << " ";
			}

		}

	}

	MPI_Finalize();

	return 0;
}
