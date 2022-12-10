cmake_minimum_required(VERSION 3.21)
project(ps-robovi)

find_package(OpenCV REQUIRED PATHS "./opencv")
include_directories("./opencv/include/opencv4") 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(testcv testcv.cpp)