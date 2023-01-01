//
// Created by lk3751 on 1/1/23.
//

#include "Canny.h"

#include <opencv2/opencv.hpp>

#include <algorithm>
#include <cassert>
#include <set>

using std::set;

Mat Canny::canny_cv2_builtin(Mat& image, double threshold_low, double threshold_high) {
    Mat edges;

    cv::Canny(image, edges, threshold_low, threshold_high);

    return edges;
}

Mat Canny::canny(Mat const &image, double threshold_low, double threshold_high)
{
    Mat edges_image;
    edges_image = gaussian_blur(image);
    auto gm = gradient_magnitude(edges_image);
    edges_image = gradient_nonmaximum_suppresion(gm);
    edges_image = double_threshold(edges_image, threshold_low, threshold_high);
    edges_image = hysteresis(image);

    return edges_image;
}

Mat Canny::gaussian_blur(Mat const &image)
{
    auto gaussian_kernel = cv::getGaussianKernel(5, 1.1);

    Mat blurred;

    cv::filter2D(image, blurred, -1, gaussian_kernel);

    return blurred;
}

Canny::AngleMagniutude Canny::gradient_magnitude(Mat const &image)
{
    Mat gradient_x, gradient_y;

    cv::Sobel(image, gradient_x, CV_32F, 1, 0);
    cv::Sobel(image, gradient_y, CV_32F, 0, 1);

    AngleMagniutude am;

    cv::cartToPolar(gradient_x, gradient_y, am.magnitude, am.angle);

    return am;
}

Mat Canny::gradient_nonmaximum_suppresion(AngleMagniutude const &angle_magniutude)
{
    auto const& angle = angle_magniutude.angle;
    auto const& magniutude = angle_magniutude.magnitude;

    auto sx = angle.cols;
    auto sy = angle.rows;

    Mat gradient_suppresed { angle.size(), CV_8U };
    
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
    
    return gradient_suppresed;
}

Mat Canny::double_threshold(Mat const &image, double threshold_low, double threshold_high)
{
    Mat thresholded { image.size(), CV_8U, cv::Scalar(Unimportant) };

    auto threshold_pixel = [&](auto x, auto y) {
        auto val = image.at<uchar>(y, x);
        if (val >= threshold_high) {
            thresholded.at<uchar>(y, x) = Strong;
        } else if (val >= threshold_low) {
            thresholded.at<uchar>(y, x) = Weak;
        }
    };

    for (int y = 0; y < image.rows; y++)
        for (int x = 0; x < image.cols; x++)
            threshold_pixel(x, y);
    
    return thresholded;
}

Mat Canny::hysteresis(Mat const &image)
{
    Mat hystereised = Mat::zeros(image.size(), CV_8U);

    auto sx = hystereised.cols;
    auto sy = hystereised.rows;

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
                hystereised.at<uchar>(y, x) = 255;

    return hystereised;
    

    return Mat();
}
