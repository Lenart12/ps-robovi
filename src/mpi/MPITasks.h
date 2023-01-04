#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <filesystem>

using std::cout;
using std::endl;

using std::string;
using std::filesystem::directory_iterator;
using std::filesystem::path;
using std::unique_ptr;


class MPITasks {
    public:
    enum Role : int;

    MPITasks(directory_iterator it, path output, int rank, int sendTo) 
        : it(it), output(output), role(((Role)rank)), sendTo(sendTo){};

    void sendImage(unique_ptr<cv::Mat>&);
    unique_ptr<cv::Mat> recvImage();
    void writeImage(int, unique_ptr<cv::Mat>&);
    unique_ptr<cv::Mat> readImage();
    Role getRole();

    private:
    directory_iterator it;
    directory_iterator end;
    path output;

    Role role;
    int sendTo;
    
};

enum MPITasks::Role : int {
    LoadImage,
    GaussianBlur,
    GradientMagnitude,
    NonMaximumSuppresion,
    DoubleThreshold,
    Hysteresis,
    SaveImage
};