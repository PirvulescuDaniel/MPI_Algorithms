#include <iostream>
#include "mpi.h"
#include <time.h>
using namespace std;

#define MSMPI_NO_DEPRECATE_20

int main() {

	std::srand(time(NULL));

	int rank, size;
	MPI_Status status;

	int studID = 3; //the studentID we want to search

	struct Student {
		int studentID;
		int year, age;
		float grade;
	};

	Student student[6];
	Student studentBuffer[6];

	int blockcounts[2];
	MPI_Datatype oldtypes[2], MPI_Student;
	MPI_Aint offset[2], extent;

	MPI_Init(NULL, NULL);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	//MPI_INT field
	blockcounts[0] = 3;
	oldtypes[0] = MPI_INT;
	offset[0] = 0;

	//MPI_FLOAT field
	MPI_Type_extent(MPI_INT, &extent);
	blockcounts[1] = 1;
	oldtypes[1] = MPI_FLOAT;
	offset[1] = 3 * extent;

	//define the struct
	MPI_Type_create_struct(3, blockcounts, offset, oldtypes, &MPI_Student);
	MPI_Type_commit(&MPI_Student);

	if (rank == 0) {

		for (int i = 0; i < 6; i++) {

			student[i].studentID = i;
			student[i].year = (1 + rand()) % 5;
			student[i].age = (19 + rand()) % 31;
			student[i].grade = (1 + rand()) % 11;

		}
		
		//assume that we have 7 processes(1 master and 6 slaves)
		int index = 0;
		for (int i = 1; i <= rank; i++) {
			MPI_Send(student + index, 2, MPI_Student, i, 1, MPI_COMM_WORLD);
			index += 2;
		}

	}

	if (rank != 0){

		MPI_Recv(studentBuffer, 2, MPI_Student, 0, 1, MPI_COMM_WORLD, &status);

		for (int i = 0; i < 2; i++) {
			if (studentBuffer[i].studentID == studID) {
				cout << "Student found!";
			}
		}

	}
	MPI_Finalize();

	return 0;
}
