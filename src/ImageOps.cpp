#include "ImageOps.h"

using std::make_unique;

unique_ptr<Mat> ImageOps::read_image(string const& file_name) {
    return make_unique<Mat>(cv::imread(file_name.c_str(), cv::IMREAD_GRAYSCALE));
}


void ImageOps::write_image(string const& file_name, Mat const& image) {
    cv::imwrite(file_name.c_str(), image);
}