//
// Created by lk3751 on 1/1/23.
//

#ifndef PS_ROBOVI_CANNY_H
#define PS_ROBOVI_CANNY_H

#include <memory>

using std::unique_ptr;

#include <opencv2/opencv.hpp>

using cv::Mat;

class Canny {
public:
    static unique_ptr<Mat> canny_cv2_builtin(Mat const& image, double threshold_low, double threshold_high);

    static unique_ptr<Mat> canny(Mat const& image, double threshold_low, double threshold_high);

    static unique_ptr<Mat> gaussian_blur(Mat const& image);

    struct AngleMagniutude {
        Mat angle;
        Mat magnitude;
    };

    static unique_ptr<AngleMagniutude> gradient_magnitude(Mat const& image);

    static unique_ptr<Mat> gradient_nonmaximum_suppresion(AngleMagniutude const& angle_magnitude);

    enum ThresholdValue : uchar {
        Unimportant = 0,
        Weak = 128,
        Strong = 255,
    };

    static unique_ptr<Mat> double_threshold(Mat const& image, double threshold_low, double threshold_high); 

    static unique_ptr<Mat> hysteresis(Mat const& image);
};


#endif //PS_ROBOVI_CANNY_H
