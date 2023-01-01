//
// Created by lk3751 on 1/1/23.
//

#ifndef PS_ROBOVI_CANNY_H
#define PS_ROBOVI_CANNY_H

#include <opencv2/opencv.hpp>


using cv::Mat;

class Canny {
public:
    static Mat canny_cv2_builtin(Mat& image, double threshold_low, double threshold_high);
};


#endif //PS_ROBOVI_CANNY_H
