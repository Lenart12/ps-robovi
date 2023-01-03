#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <filesystem>

#include <cassert>
#include <mpi.h>


using std::vector;
using std::array;
using std::string;
using std::cout;
using std::endl;
using std::filesystem::path;

void mpi_main(size_t node_count, path input_directory, path output_directory, size_t images_count) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    assert(node_count == size);

    cout << "Mpi [" << rank << "/" << size << "]" << endl;
}