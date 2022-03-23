#include <iostream>
#include "mpi.h"
using namespace std;

int main() {

	int rank, size;
	int n, vSize;
	int v[] = { 1, 12, 25 , 36, 12, 41, 1, 77};
	int no_of_elements, low, high;
	int temp_index, index;
	MPI_Status status;

	vSize = sizeof(v) / sizeof(v[0]);

	n = 1; //element to found

	MPI_Init(NULL, NULL);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0) {
		MPI_Bcast(v, vSize, MPI_INT, 0, MPI_COMM_WORLD);
	}

	no_of_elements = vSize / 4; //the number of items checked by each process(assume that we have 4 processes)
	
	//compute the edgeds of the intervals
	low = rank * no_of_elements;
	high = low + no_of_elements;

	temp_index = -1;
	for (int i = low; i < high; i++) {

		if (v[i] == n) {
			temp_index = i;
		}

	}

	MPI_Reduce(&temp_index, &index, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if (rank == 0) {

		if (index == -1) {
			cout << "Not found!";
		}
		else {
			cout << "The index of the element: " << index;
		}

	}

	MPI_Finalize();

	return 0;
}
