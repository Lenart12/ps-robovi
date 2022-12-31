#!/bin/bash

module load "CMake/3.23.1-GCCcore-11.3.0"
module load "OpenMPI/4.1.4-GCC-11.3.0"

C_COMPILER=mpicc
CXX_COMPILER=mpicxx
SRC_DIR="$(pwd)"
BUILD_DIR="$SRC_DIR/cmake-build"

cmake -DCMAKE_C_COMPILER=mpicc -DCMAKE_CXX_COMPILER=mpicxx -B"$BUILD_DIR" -S"$SRC_DIR"