#!/bin/bash

BUILD_DIR="build"
PROJECT_NAME="optimizer"

mkdir -p $BUILD_DIR

cmake -B $BUILD_DIR -S .
cmake --build $BUILD_DIR

exec $BUILD_DIR/$PROJECT_NAME