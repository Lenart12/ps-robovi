#include "MPITasks.h"
#include <string>
#include <array>
#include <iostream>
#include <filesystem>
#include <mpi.h>
#include "../ImageOps.h"
#include "../Canny.h"
#include <opencv2/opencv.hpp>

using std::byte;
using std::unique_ptr;
using std::string;
using std::filesystem::directory_iterator;
using std::filesystem::path;
using std::unique_ptr;
using Role = MPITasks::Role;


void MPITasks::sendImage(unique_ptr<cv::Mat> &image)
{
    cv::Mat i = *image;
    int size = i.total() * i.elemSize();
    int *dim = new int[4]{i.rows, i.cols, size, i.type()};

    MPI_Send(dim, 4, MPI_INT, sendTo, 0, MPI_COMM_WORLD);
    MPI_Send(i.data, size, MPI_BYTE, sendTo, 0, MPI_COMM_WORLD);
    free(dim);
}

unique_ptr<cv::Mat> MPITasks::recvImage()
{
    int *dim = new int[4];
    unique_ptr<cv::Mat> image;
    MPI_Recv(dim, 4, MPI_INTEGER, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    if(dim[0] == -1 || dim[1] == -1){
        if(sendTo >= 0)
            MPI_Send(dim, 4, MPI_INT, sendTo, 0, MPI_COMM_WORLD);
        return nullptr;
    }

    const int size = dim[2];
    byte *bytes = new byte[size];

    MPI_Recv(bytes, size, MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    image = unique_ptr<cv::Mat>(new Mat(dim[0], dim[1], dim[3], bytes));

    return image;
}

void MPITasks::writeImage(int num, unique_ptr<cv::Mat> &image)
{
    ImageOps::write_image(output.string() + "/slika" + std::to_string(num) + ".jpg", *image);
}

unique_ptr<cv::Mat> MPITasks::readImage()
{
    if (it == end)
    {
        int *dim = new int[4] {-1, -1, -1, -1};
        printf("KER JE KONEC POSILJAM ZNAK DA JE KONEC NA %d\n", sendTo);
        MPI_Send(dim, 4, MPI_INT, sendTo, 0, MPI_COMM_WORLD);
        return nullptr;
    }

    auto const &entry = *it;
    unique_ptr<cv::Mat> image = ImageOps::read_image(entry.path().string());

    ++it;
    return image;
}

Role MPITasks::getRole(){
    return role;
}