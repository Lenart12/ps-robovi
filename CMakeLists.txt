cmake_minimum_required(VERSION 3.21)
project(ps-robovi)

set(CMAKE_CXX_STANDARD 20)


find_package( OpenCV REQUIRED )                              
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(
    ps-robovi
    src/main.cpp
    src/mpi_main.cpp
    src/pthread_main.cpp
    src/serial_main.cpp
    src/Canny.cpp
    src/Canny.h
    src/ImageOps.cpp
    src/ImageOps.h
    src/ScopeTimer.cpp
    src/ScopeTimer.h
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
target_link_libraries(ps-robovi PUBLIC pthread ${OpenCV_LIBS})
