#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <filesystem>

#include <cassert>
#include <mpi.h>

#include "MPITasks.h"
#include "../ImageOps.h"
#include "../Canny.h"
#include "../ScopeTimer.h"


using std::byte;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::filesystem::directory_iterator;
using std::filesystem::path;

using Role = MPITasks::Role;

unique_ptr<MPITasks> t;

bool findEdges()
{
    static int counter = 0;

    switch (t->getRole())
    {
    case Role::LoadImage:
    {
        // Beri sliko
        unique_ptr<cv::Mat> image = t->readImage();
        if (image == nullptr)
            return false;

        t->sendImage(image);
        return true;
    }

    case Role::GaussianBlur:
    {
        // Gaussian blur
        unique_ptr<cv::Mat> image = t->recvImage();
        if (image == nullptr)
            return false;

        image = Canny::gaussian_blur(*image);
        t->sendImage(image);
        break;
    }

    case Role::GradientMagnitude:
    {
        // Racunanje gradienta
        unique_ptr<cv::Mat> image = t->recvImage();
        if (image == nullptr)
            return false;

        auto am = Canny::gradient_magnitude(*image);

        unique_ptr<cv::Mat> pa = unique_ptr<cv::Mat>(new Mat(am->angle));
        t->sendImage(pa);

        unique_ptr<cv::Mat> pm = unique_ptr<cv::Mat>(new Mat(am->magnitude));
        t->sendImage(pm);
        break;
    }

    case Role::NonMaximumSuppresion:
    {
        // Tansanje robov
        unique_ptr<cv::Mat> pa = t->recvImage();
        if (pa == nullptr)
            return false;
        unique_ptr<cv::Mat> pm = t->recvImage();

        Canny::AngleMagniutude am{*pa, *pm};
        unique_ptr<cv::Mat> image = Canny::gradient_nonmaximum_suppresion(am);
        t->sendImage(image);
        break;
    }

    case Role::DoubleThreshold:
    {
        // Dvojno upragovanje
        unique_ptr<cv::Mat> image = t->recvImage();
        if (image == nullptr)
            return false;

        image = Canny::double_threshold(*image, 50, 150);
        t->sendImage(image);
        break;
    }

    case Role::Hysteresis:
    {
        // Sledenje robovom s pomočjo histereze
        unique_ptr<cv::Mat> image = t->recvImage();
        if (image == nullptr)
            return false;

        image = Canny::hysteresis(*image);
        t->sendImage(image);
        break;
    }

    case Role::SaveImage:
    {
        // Shrani sliko
        unique_ptr<cv::Mat> image = t->recvImage();
        if (image == nullptr)
            return false;

        t->writeImage(counter++, image);
        break;
    }
    }

    return true;
}

void mpi_main(size_t node_count, path input_directory, path output_directory, size_t images_count)
{
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    assert(node_count == size);

    cout << "Mpi [" << rank << "/" << size << "]" << endl;

    directory_iterator it;
    if (rank == 0)
    {
        it = directory_iterator(input_directory);
    }

    int sendTo = (rank + 1 == size) ? -1 : rank + 1;
    t = unique_ptr<MPITasks>(new MPITasks(it, output_directory, rank, sendTo));

    {
    auto time = t->casi.time_hysteresis();
    while (findEdges())
    {
    };
    }
    cout << "PROCES " << rank << " je koncal" << endl;

    auto posiljanje = t->casi.read_image.count();
    auto prejemanje = t->casi.write_image.count();
    auto vse = t->casi.hysteresis.count();
    auto racunanje = vse - posiljanje - prejemanje;

    cout << "CAS ZA POSILJANJE : " << posiljanje << "ms + CASI ZA PREJEMANJE " << prejemanje << "ms = "<< (posiljanje + prejemanje) << endl;
    cout << "SKUPEN CAS : " << vse << "ms" << endl;
    cout << "RACUNANJE : " << racunanje << "ms" << endl;

}