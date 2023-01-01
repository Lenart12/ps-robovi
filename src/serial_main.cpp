#include <vector>
#include <string>
#include <array>
#include <iostream>

#include "ImageOps.h"
#include "Canny.h"

using std::vector;
using std::array;
using std::string;
using std::cout;
using std::endl;


void serial_main(vector<string> args) {
    auto lena = ImageOps::read_image("images/lena.png");

    auto lena_edges = Canny::canny_cv2_builtin(lena, 50, 200);

    ImageOps::write_image("output/lena.png", lena_edges);
}


