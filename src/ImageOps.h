//
// Created by lk3751 on 1/1/23.
//

#ifndef PS_ROBOVI_IMAGEOPS_H
#define PS_ROBOVI_IMAGEOPS_H

#include <string>
#include <memory>
using std::string;
using std::unique_ptr;

#include <opencv2/opencv.hpp>

using cv::Mat;

class ImageOps {
public:
    static unique_ptr<Mat> read_image(string const& file_name);
    static void write_image(string const& file_name, Mat const& image);
};


#endif //PS_ROBOVI_IMAGEOPS_H
