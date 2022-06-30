#!/bin/bash

cmake -DCMAKE_BUILD_TYPE=Debug -S . -B ./build
cd ./build
make
./blare-testing/ArrayListTest
echo "Exit code of ArrayListTest: $?"