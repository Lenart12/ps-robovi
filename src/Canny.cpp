//
// Created by lk3751 on 1/1/23.
//

#include "Canny.h"

#include <opencv2/opencv.hpp>

#include <algorithm>
#include <cassert>
#include <set>

#include "ScopeTimer.h"

using std::set;
using std::make_unique;

// Change commented out line to enable internak timers
#define CannyTimer()
// #define CannyTimer() FunctionTimer()

unique_ptr<Mat> Canny::canny_cv2_builtin(Mat const& image, double threshold_low, double threshold_high) {
    CannyTimer();

    auto edges = make_unique<Mat>();

    cv::Canny(image, *edges, threshold_low, threshold_high);

    return edges;
}

unique_ptr<Mat> Canny::canny(Mat const &image, double threshold_low, double threshold_high)
{
    CannyTimer();

    auto blurred = Canny::gaussian_blur(image);
    auto gm = Canny::gradient_magnitude(*blurred);
    auto nonmax = Canny::gradient_nonmaximum_suppresion(*gm);
    auto threshold = Canny::double_threshold(*nonmax, threshold_low, threshold_high);
    auto hysteresis = Canny::hysteresis(*threshold);

    return hysteresis;
}

unique_ptr<Mat> Canny::gaussian_blur(Mat const &image)
{
    CannyTimer();
    auto gaussian_kernel = cv::getGaussianKernel(5, 1.1);

    auto blurred = make_unique<Mat>();

    cv::filter2D(image, *blurred, -1, gaussian_kernel);

    return blurred;
}

unique_ptr<Canny::AngleMagniutude> Canny::gradient_magnitude(Mat const &image)
{
    CannyTimer();
    Mat gradient_x, gradient_y;

    cv::Sobel(image, gradient_x, CV_32F, 1, 0);
    cv::Sobel(image, gradient_y, CV_32F, 0, 1);

    auto am = make_unique<AngleMagniutude>();

    cv::cartToPolar(gradient_x, gradient_y, am->magnitude, am->angle);

    return am;
}

unique_ptr<Mat> Canny::gradient_nonmaximum_suppresion(AngleMagniutude const &angle_magniutude)
{
    CannyTimer();
    auto const& angle = angle_magniutude.angle;
    auto const& magniutude = angle_magniutude.magnitude;

    auto sx = angle.cols;
    auto sy = angle.rows;

    auto gradient_suppresed_ptr = make_unique<Mat>(angle.size(), CV_8U);
    auto& gradient_suppresed = *gradient_suppresed_ptr.get();
    
    auto suppress_pixel = [&](auto x, auto y) {
        auto grad = angle.at<float>(y, x);
        if (grad < 0) grad += CV_PI;
        auto dir = (int)std::floor(grad / (CV_PI / 8)) % 8;

        int nx, ny;
        #define check_offset(off_x, off_y) \
            nx = x + off_x; \
            ny = y + off_y; \
            if (nx >= 0 && nx < sx && ny >= 0 && ny < sy &&  \
                magniutude.at<float>(y, x) <= magniutude.at<float>(ny, nx) \
            ) { gradient_suppresed.at<uchar>(y, x) = 0; return; }

        switch(dir) {
            // Horizontal
            case 0:
            case 7:
                check_offset(1, 0)
                check_offset(-1, 0)    
                break;
            //  / angled
            case 1:
            case 2:
                check_offset(-1, -1)
                check_offset(1, 1)  
                break;
            // Vertical
            case 3:
            case 4:
                check_offset(0, 1)
                check_offset(0, -1)  
                break;
            //  \ angled
            case 5:
            case 6:
                check_offset(-1, 1)
                check_offset(1, -1)  
                break;
            default:
                assert(!"Invalid gradient angle");
        }
        #undef check_offset

        gradient_suppresed.at<uchar>(y, x) = (uchar)std::clamp(magniutude.at<float>(y, x), 0.0f, 255.0f);
    };

    for (int y = 0; y < sy; y++)
        for (int x = 0; x < sx; x++)
            suppress_pixel(x, y);
    
    return gradient_suppresed_ptr;
}

unique_ptr<Mat> Canny::double_threshold(Mat const &image, double threshold_low, double threshold_high)
{
    CannyTimer();
    auto thresholded = make_unique<Mat>(image.size(), CV_8U, cv::Scalar(Unimportant));

    auto threshold_pixel = [&](auto x, auto y) {
        auto val = image.at<uchar>(y, x);
        if (val >= threshold_high) {
            thresholded->at<uchar>(y, x) = Strong;
        } else if (val >= threshold_low) {
            thresholded->at<uchar>(y, x) = Weak;
        }
    };

    for (int y = 0; y < image.rows; y++)
        for (int x = 0; x < image.cols; x++)
            threshold_pixel(x, y);
    
    return thresholded;
}

unique_ptr<Mat> Canny::hysteresis(Mat const &image)
{
    CannyTimer();
    auto hystereised = make_unique<Mat>(image.size(), CV_8U, cv::Scalar(0));

    auto sx = hystereised->cols;
    auto sy = hystereised->rows;

    Mat groups;
    // NOTE: This treats both weak and strong values as members of the same component
    cv::connectedComponents(image, groups);


    // First pass: Mark connected weak groups
    set<int> connected_groups;
    for (int y = 0; y < sy; y++)
        for (int x = 0; x < sx; x++)
            if (image.at<uchar>(y, x) == Strong)
                connected_groups.insert(groups.at<int>(y, x));

    // Second pass: Mark connected groups as edges
    for (int y = 0; y < sy; y++)
        for (int x = 0; x < sx; x++)
            if (connected_groups.count(groups.at<int>(y, x)) > 0)
                hystereised->at<uchar>(y, x) = 255;

    return hystereised;
}
