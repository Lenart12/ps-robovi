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

    static Mat canny(Mat const& image, double threshold_low, double threshold_high);

    static Mat gaussian_blur(Mat const& image);

    struct AngleMagniutude {
        Mat angle;
        Mat magnitude;
    };

    static AngleMagniutude gradient_magnitude(Mat const& image);

    static Mat gradient_nonmaximum_suppresion(AngleMagniutude const& angle_magnitude);

    enum ThresholdValue : uchar {
        Unimportant = 0,
        Weak = 128,
        Strong = 255,
    };

    static Mat double_threshold(Mat const& image, double threshold_low, double threshold_high); 

    static Mat hysteresis(Mat const& image);
};


#endif //PS_ROBOVI_CANNY_H
