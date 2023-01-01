//
// Created by lk3751 on 1/1/23.
//

#include "Canny.h"

#include <opencv2/opencv.hpp>

Mat Canny::canny_cv2_builtin(Mat& image, double threshold_low, double threshold_high) {
    Mat edges;

    cv::Canny(image, edges, threshold_low, threshold_high);

    return edges;
}

