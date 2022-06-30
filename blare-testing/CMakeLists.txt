cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)
project(blare-tests VERSION 0.1.0 LANGUAGES CXX C)


add_executable(ArrayListTest "${CMAKE_CURRENT_SOURCE_DIR}/container/ArrayListTest.cpp")
target_include_directories(ArrayListTest PRIVATE "${BLARE_CMAKE_DIR}/${BLARE_INCLUDE_DIR}")
add_test(${PROJECT_NAME}^ArrayList ArrayListTest)