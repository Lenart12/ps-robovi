#include <vector>
#include <string>
#include <array>
#include <iostream>
#include <filesystem>

#include "ImageOps.h"
#include "ScopeTimer.h"
#include "Canny.h"

using std::vector;
using std::array;
using std::string;
using std::cout;
using std::endl;
using std::filesystem::path;
using std::filesystem::directory_iterator;

void test_lena_png() {
    auto lena = ImageOps::read_image("images/lena.png");
    ImageOps::write_image("output/lena.png", *lena);

    auto lena_edges = Canny::canny_cv2_builtin(*lena, 50, 200);
    ImageOps::write_image("output/lena_canny_cv2.png", *lena_edges);

    auto lena_blurred = Canny::gaussian_blur(*lena);
    ImageOps::write_image("output/lena_blurred.png", *lena_blurred);

    auto gm = Canny::gradient_magnitude(*lena_blurred);
    ImageOps::write_image("output/lena_angle.png", gm->angle);
    ImageOps::write_image("output/lena_magnitude.png", gm->magnitude);

    auto lena_nonmax = Canny::gradient_nonmaximum_suppresion(*gm);
    ImageOps::write_image("output/lena_nonmax.png", *lena_nonmax);

    auto lena_thr = Canny::double_threshold(*lena_nonmax, 50, 100);
    ImageOps::write_image("output/lena_thr.png", *lena_thr);

    auto lena_hyst = Canny::hysteresis(*lena_thr);
    ImageOps::write_image("output/lena_hyst.png", *lena_hyst);
}


void serial_main(vector<string> args) {
    FunctionTimer();

    path input_directory { "images" };
    path output_directory { "output" };
    
    for (auto const& entry : directory_iterator(input_directory)) {
        ScopeTimer image_timer {entry.path(), true};
        auto image = ImageOps::read_image(entry.path());
        auto canny_image = Canny::canny(*image, 20, 150);
        auto output_path = output_directory / entry.path().filename();
        ImageOps::write_image(output_path.string(), *canny_image);
    }
}


