cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)
project(blare-tests VERSION 0.1.0 LANGUAGES CXX C)

include_directories("${BLARE_CMAKE_DIR}/${BLARE_INCLUDE_DIR}")

add_executable(ArrayListTest "${CMAKE_CURRENT_SOURCE_DIR}/container/ArrayListTest.cpp")
add_executable(ArrayTest "${CMAKE_CURRENT_SOURCE_DIR}/container/ArrayTest.cpp")

add_test(${PROJECT_NAME}^ArrayList ArrayListTest)
add_test(${PROJECT_NAME}^Array ArrayTest)