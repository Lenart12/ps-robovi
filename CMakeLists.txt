cmake_minimum_required(VERSION 3.21)
project(ps-robovi)

set(CMAKE_CXX_STANDARD 20)


FIND_PACKAGE( OpenCV REQUIRED )                              
INCLUDE_DIRECTORIES( ${OpenCV_INCLUDE_DIRS} )

add_executable(
    ps-robovi
    src/main.cpp
    src/canny.cpp
    src/stb/stb_image_write.h
    src/stb/stb_image.h
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
target_link_libraries(ps-robovi PUBLIC pthread)