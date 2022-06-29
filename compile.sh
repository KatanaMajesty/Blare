#!/bin/bash

cmake -DCMAKE_BUILD_TYPE=Debug -S . -B ./build
cd ./build
make
./blare-testing/ArraylistTest
echo "Exit code of ArraylistTest: $?"