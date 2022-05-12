#include "mpi.h"
#include <iostream>
#define NPROCS 16

using namespace std;

int searchMin(int v[], int n){
	int min = v[0];
	for (int i = 0; i < n; i++) {
		min = ::min(min, v[i]);
	}
	return min;
}

int searchMax(int v[], int n){
	int max = v[0];
	for (int i = 0; i < n; i++) {
		max = ::max(max, v[i]);
	}
	return max;
}

int main() {
	int rank, sum = 0, prod = 1, min = 0, max = 0, numtasks, new_rank;

	int rnk1[4] = { 0,1,2,3 };
	int rnk2[4] = { 4,5,6,7 };
	int rnk3[4] = { 8,9,10,11 };
	int rnk4[4] = { 12,13,14,15 };

	int array[8] = { 1, 2, 3, 4, 5, 6, 7, 8 }, recv_array[2];
	int n = sizeof(array) / sizeof(rnk1);

	int c[4];

	MPI_Group orig_group, new_group;
	MPI_Comm new_comm1, new_comm2, new_comm3, new_comm4;

	MPI_Init(NULL, NULL);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	MPI_Comm_group(MPI_COMM_WORLD, &orig_group);
	
	if (0 <= rank && rank < 4) {
		MPI_Group_incl(orig_group, NPROCS / 4, rnk1, &new_group);
	}
	else if (4 <= rank && rank < 8) {
		MPI_Group_incl(orig_group, NPROCS / 4, rnk2, &new_group);
	}
	else if (8 <= rank && rank < 12) {
		MPI_Group_incl(orig_group, NPROCS / 4, rnk3, &new_group);
	}
	else {
		MPI_Group_incl(orig_group, NPROCS / 4, rnk4, &new_group);
	}
	
	MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm1);
	MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm2);
	MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm3);
	MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm4);

	MPI_Group_rank(new_group, &new_rank);

	//compute sum
	MPI_Scatter(array, n, MPI_INT, recv_array, n, MPI_INT, 0, new_comm1);
	for (int i = 0; i < n; i++)
		sum += recv_array[i];

	MPI_Gather(&sum, 1, MPI_INT, c, 1, MPI_INT, 0, new_comm1);
	if (new_rank == 0)
	{
		sum = 0;
		for (int i = 0; i < NPROCS / 4; i++)
			sum += c[i];
	}

	//compute product
	MPI_Scatter(array, n, MPI_INT, recv_array, n, MPI_INT, 0, new_comm2);
	for (int i = 0; i < n; i++)
		prod *= recv_array[i];

	MPI_Gather(&prod, 1, MPI_INT, c, 1, MPI_INT, 0, new_comm2);
	if (new_rank == 0)
	{
		prod = 1;
		for (int i = 0; i < NPROCS / 4; i++)
			prod *= c[i];
	}

	//search for min
	MPI_Scatter(array, n, MPI_INT, recv_array, n, MPI_INT, 0, new_comm3);
	min = searchMin(recv_array, n);

	MPI_Gather(&min, 1, MPI_INT, c, 1, MPI_INT, 0, new_comm3);
	if (new_rank == 0)
	{
		min = searchMin(c, NPROCS / 4);
	}

	//search for max
	MPI_Scatter(array, n, MPI_INT, recv_array, n, MPI_INT, 0, new_comm4);
	max = searchMax(recv_array, n);

	MPI_Gather(&max, 1, MPI_INT, c, 1, MPI_INT, 0, new_comm4);
	if (new_rank == 0)
	{
		max = searchMax(c, NPROCS / 4);
	}

	if (new_rank == 0)
	{
		cout << "suma:" << sum << endl;
		cout << "produsul:" << prod << endl;
		cout << "minimul: " << min << endl;
		cout << "maximul:" << max << endl;
	}



	MPI_Finalize();
}
