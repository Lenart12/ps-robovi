cmake_minimum_required(VERSION 3.21)
project(ps-robovi)

set(CMAKE_CXX_STANDARD 20)

add_executable(
    ps-robovi
    src/main.cpp
    src/canny.cpp
    src/stb/stb_image_write.h
    src/stb/stb_image.h
)
