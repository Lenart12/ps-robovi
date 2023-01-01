#include <vector>
#include <string>
#include <iostream>

#include <mpi.h>
#include <opencv2/core/version.hpp>

#include "Canny.h"

using std::vector;
using std::array;
using std::string;
using std::cout;
using std::cerr;
using std::endl;


void pthread_main(vector<string> args) ;
void mpi_main(vector<string> args) ;
void serial_main(vector<string> args) ;


void ps_main(vector<string> args) {
    if (args.size() == 0) {
        cerr << "Manjka argument [pthread/mpi/serial]" << endl;
        return;
    }

    args.erase(args.begin());

    if (args[0] == "mpi") {
        mpi_main(std::move(args));
    } else if (args[0] == "pthread") {
        pthread_main(std::move(args));
    } else if (args[0] == "serial") {
        serial_main(std::move(args));
    } else {
        cerr << "Prvi argument ni [pthread/mpi/serial]" << endl;
        return;
    }
}

int main(int argc, char *argv[])
{
    vector<string> args;

    MPI_Init(&argc, &argv);

    for (int i = 1; i < argc; i++) {
        args.emplace_back(argv[i]);
    }
    ps_main(std::move(args));

    MPI_Finalize();
    return 0;
}
