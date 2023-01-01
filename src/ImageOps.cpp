#include "ImageOps.h"


Mat ImageOps::read_image(string const& file_name) {
    return cv::imread(file_name.c_str(), cv::IMREAD_GRAYSCALE);
}


void ImageOps::write_image(string const& file_name, Mat const& image) {
    cv::imwrite(file_name.c_str(), image);
}