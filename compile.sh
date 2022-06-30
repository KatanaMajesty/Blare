#!/bin/bash

cmake -DCMAKE_BUILD_TYPE=Debug -S . -B ./build
mkdir ./build
cmake --build ./build -- -j2
cd ./build
make
ctest -j2