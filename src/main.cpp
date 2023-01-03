#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

#include <mpi.h>
#include <opencv2/core/version.hpp>

#include "Canny.h"
using std::vector;
using std::array;
using std::string;
using std::cout;
using std::cerr;
using std::filesystem::path;
using std::filesystem::is_directory;
using std::endl;


void pthread_main(size_t thread_count, path input_directory, path output_directory, size_t images_count);
void mpi_main(size_t node_count, path input_directory, path output_directory, size_t images_count);
void serial_main(path input_directory, path output_directory, size_t images_count);


void ps_main(vector<string> args) {
    if (args.size() == 0) {
        cerr << "Manjka argument [pthread/mpi/serial]" << endl;
        return;
    }

    auto ps_main = std::move(args[0]);
    args.erase(args.begin());

    path input_directory = "./images";
    path output_directory = "./output";
    size_t task_count = 8;
    size_t images_count = -1;

    if (ps_main != "serial" && !args.empty()) {
        try {
            task_count = std::stoi(args[0]);
        } catch (...) {
            cerr << "Task count is not a number" << endl;
            return;
        }

        args.erase(args.begin());
    }

    if (!args.empty()) {
        input_directory = args[0];
        args.erase(args.begin());
    }

    if (!args.empty()) {
        output_directory = args[0];
        args.erase(args.begin());
    }

    if (!args.empty()) {
        try {
            images_count = std::stoi(args[0]);
        } catch (...) {
            cerr << "Images count is not a number" << endl;
            return;
        }

        args.erase(args.begin());
    }

    if (!args.empty()) {
        cerr << "Unexpected argument " << args[0] << endl;
        return;
    }

    if (!is_directory(input_directory)) {
        cerr << input_directory << " is not a directory" << endl;
        return;
    }

    if (!is_directory(output_directory)) {
        cerr << output_directory << " is not a directory" << endl;
        return;
    }

    if (ps_main == "mpi") {
        mpi_main(task_count, input_directory, output_directory, images_count);
    } else if (ps_main == "pthread") {
        pthread_main(task_count, input_directory, output_directory, images_count);
    } else if (ps_main == "serial") {
        serial_main(input_directory, output_directory, images_count);
    } else {
        cerr << "Prvi argument ni [pthread/mpi/serial] [" << ps_main << "]" << endl;
        return;
    }
}

int main(int argc, char *argv[])
{
    vector<string> args;

    MPI_Init(&argc, &argv);

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg.empty()) continue;
        args.push_back(arg);
    }
    ps_main(std::move(args));

    MPI_Finalize();
    return 0;
}
