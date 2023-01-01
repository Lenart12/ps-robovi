#include <cstdio>

#include <vector>
#include <string>
#include <array>
#include <iostream>

#include <mpi.h>
#include <pthread.h>

#include <opencv2/core/version.hpp>

using std::vector;
using std::array;
using std::string;
using std::cout;
using std::endl;

extern "C" void* test_thread(void* _arg) {
    auto id = reinterpret_cast<size_t>(_arg);
    cout << "Pthread " << id << endl;
    
    return nullptr;
}

void ps_main(vector<string> args) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    cout << "OpenCV " << CV_VERSION_MAJOR    << " "
                      << CV_VERSION_MINOR    << " "
                      << CV_VERSION_REVISION << endl;

    cout << "Args " << args.size() << endl;

    cout << "Mpi [" << rank << "/" << size << "]" << endl;

    array<pthread_t, 5> threads;

    for (size_t i = 0; i < threads.size(); i++) {
        pthread_create(&threads[i], nullptr, test_thread, reinterpret_cast<void*>(i));
    }

    for (auto& thread : threads) {
        pthread_join(thread, nullptr);
    }

    cout << "Done!" << endl;
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
