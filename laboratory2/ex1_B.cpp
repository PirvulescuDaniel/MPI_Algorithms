#include <iostream>
#include "mpi.h"
using namespace std;

int main() {

	int rank, size;
	int n, vSize;
	int v[] = { 1, 12, 25 , 36, 12, 41, 1, 77};
	int u[10];
	int no_of_elements_to_send;
	MPI_Status status;

	int sendBuffer[50];
	int reciveBuffer[50];

	vSize = sizeof(v) / sizeof(v[0]);
	no_of_elements_to_send = vSize / 4; //assume that we have 4 processes

	n = 1; //element to found

	MPI_Init(NULL, NULL);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Scatter(v, no_of_elements_to_send, MPI_INT, u, no_of_elements_to_send, MPI_INT, 0, MPI_COMM_WORLD);
	
	for (int i = 0; i < no_of_elements_to_send; i++) {

		if (u[i] == n) {

			if (i == 0) {
				sendBuffer[i] = rank * no_of_elements_to_send;
			}
			else {
				sendBuffer[i] = rank * no_of_elements_to_send + 1;
			}

		}
		else {
			sendBuffer[i] = -1;
		}

	}

	MPI_Gather(sendBuffer, no_of_elements_to_send, MPI_INT, reciveBuffer, no_of_elements_to_send, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0) {

		for (int i = 0; i < vSize; i++) {
			if (reciveBuffer[i] != -1) {
				cout << reciveBuffer[i] << " ";
			}
		}

	}
	
	MPI_Finalize();

	return 0;
}
