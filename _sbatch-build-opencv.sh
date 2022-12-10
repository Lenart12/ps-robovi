#!/bin/bash
#SBATCH --job-name=build_opencv
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --cpus-per-task=16
#SBATCH --reservation=fri
#SBATCH --time=05:00
#SBATCH --output=opencv-build/build-opencv.out

ROOT="$(pwd)"
OPENCV_DIR="$ROOT/opencv-4.x"
OPENCV_TMP="$ROOT/opencv-tmp.zip"
BUILD_DIR="$ROOT/opencv-build"
INSTALL_DIR="$ROOT/opencv"

echo "Load modules"
module load "CMake/3.21.1-GCCcore-11.2.0" || exit 1

echo "Configuring"
cmake "$OPENCV_DIR" || exit 1

mkdir -p "$BUILD_DIR" && cd "$BUILD_DIR"
cmake -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR" "$OPENCV_DIR" || exit 1

echo "Building inside $(pwd)"
cmake --build . -j16 && echo "Build successful!"

echo "Press Ctrl-C to exit"