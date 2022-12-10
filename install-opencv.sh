#!/bin/bash

OPENCV_SRC="https://github.com/opencv/opencv/archive/4.x.zip"

function echo_red() {
    echo -e "\033[0;31m$1\033[0m"
}

ROOT="$(pwd)"
OPENCV_DIR="$ROOT/opencv-4.x"
OPENCV_TMP="$ROOT/opencv-tmp.zip"
BUILD_DIR="$ROOT/opencv-build"

echo_red "Downloading opencv.zip"
wget -O "$OPENCV_TMP" "$OPENCV_SRC" || exit 1
echo_red "Extracting opencv.zip"
unzip "$OPENCV_TMP" -d "$ROOT" || exit 1
rm "$OPENCV_TMP_DIR"


echo_red "Building"
echo "" > "$BUILD_DIR/build-opencv.out"
sbatch "$ROOT/_sbatch-build-opencv.sh" || exit 1
tail -f "$BUILD_DIR/build-opencv.out"

cd "$ROOT"