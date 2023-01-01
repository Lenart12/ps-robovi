#include <vector>
#include <string>
#include <array>
#include <iostream>

#include <mpi.h>


using std::vector;
using std::array;
using std::string;
using std::cout;
using std::endl;


void mpi_main(vector<string> args) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    cout << "Mpi [" << rank << "/" << size << "]" << endl;
}