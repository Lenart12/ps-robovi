#include <opencv2/opencv.hpp>
#include <opencv2/core/version.hpp>

int main(int argc, char const *argv[])
{
    printf("OpenCV: %d %d %d", CV_VERSION_MAJOR, CV_VERSION_MINOR, CV_VERSION_REVISION);
    return 0;
}
