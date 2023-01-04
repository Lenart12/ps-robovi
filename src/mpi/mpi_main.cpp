#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <filesystem>

#include <cassert>
#include <mpi.h>

#include "../ImageOps.h"
#include "../Canny.h"

using std::byte;
using std::cout;
using std::decay_t;
using std::endl;
using std::is_same_v;
using std::make_shared;
using std::string;
using std::stringstream;
using std::vector;
using std::filesystem::directory_iterator;
using std::filesystem::path;

directory_iterator it;
directory_iterator end;
path output;

int rank;
int size;

int counter{0};

void sendImage(unique_ptr<cv::Mat> &image)
{
    cv::Mat i = *image;
    int size = i.total() * i.elemSize();
    int *dim = new int[4]{i.rows, i.cols, size, i.type()};

    MPI_Send(dim, 4, MPI_INT, rank + 1, rank, MPI_COMM_WORLD);
    MPI_Send(i.data, size, MPI_BYTE, rank + 1, rank, MPI_COMM_WORLD);
    free(dim);
}

unique_ptr<cv::Mat> getImage()
{

    int *dim = new int[4];
    MPI_Recv(dim, 4, MPI_INTEGER, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    const int size = dim[2];
    byte *bytes = new byte[size];

    MPI_Recv(bytes, size, MPI_BYTE, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    unique_ptr<cv::Mat> image = unique_ptr<cv::Mat>(new Mat(dim[0], dim[1], dim[3], bytes));

    return image;
}

void writeImage(int num, unique_ptr<cv::Mat> &image)
{
    ImageOps::write_image(output.string() + "/slika" + std::to_string(num) + ".jpg", *image);
}

unique_ptr<cv::Mat> readImage()
{
    if (it == end)
    {
        return unique_ptr<cv::Mat>(nullptr);
    }

    auto const &entry = *it;
    cout << entry.path().string() << endl;
    unique_ptr<cv::Mat> image = ImageOps::read_image(entry.path().string());

    ++it;
    return image;
}

bool findEdges()
{

    switch (rank)
    {
    case 0:
    {
        // Beri sliko
        unique_ptr<cv::Mat> image = readImage();
        sendImage(image);
        break;
    }

    case 1:
    {
        // Gaussian blur
        unique_ptr<cv::Mat> image = getImage();
        image = Canny::gaussian_blur(*image);
        sendImage(image);
        break;
    }

    case 2:
    {
        // Racunanje gradienta
        unique_ptr<cv::Mat> image = getImage();
        auto am = Canny::gradient_magnitude(*image);

        unique_ptr<cv::Mat> pa = unique_ptr<cv::Mat>(new Mat(am->angle));
        sendImage(pa);

        unique_ptr<cv::Mat> pm = unique_ptr<cv::Mat>(new Mat(am->magnitude));
        sendImage(pm);
        break;
    }

    case 3:
    {
        // Tansanje robov
        unique_ptr<cv::Mat> pa = getImage();
        unique_ptr<cv::Mat> pm = getImage();

        Canny::AngleMagniutude am{*pa, *pm};
        unique_ptr<cv::Mat> image = Canny::gradient_nonmaximum_suppresion(am);
        sendImage(image);
        break;
    }

    case 4:
    {
        // Dvojno upragovanje
        unique_ptr<cv::Mat> image = getImage();
        image = Canny::double_threshold(*image, 50, 150);
        sendImage(image);
        break;
    }

    case 5:
    {
        // Sledenje robovom s pomočjo histereze
        unique_ptr<cv::Mat> image = getImage();
        image = Canny::hysteresis(*image);
        sendImage(image);
        break;
    }

    case 6:
    {
        // Shrani sliko
        unique_ptr<cv::Mat> image = getImage();
        writeImage(6, image);
        break;
    }
    }

    return false;
}

void mpi_main(size_t node_count, path input_directory, path output_directory, size_t images_count)
{
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    assert(node_count == size);

    cout << "Mpi [" << rank << "/" << size << "]" << endl;

    output = output_directory;
    if (rank == 0)
    {
        it = directory_iterator(input_directory);
    }

    findEdges();
    cout << "NIT " << rank << " je koncala" << endl;
}