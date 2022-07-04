cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)
project(blare VERSION 0.1.0 LANGUAGES CXX C)

set(BLARE_TESTS ON)
set(BLARE_TESTING_DIR "blare-testing")
set(BLARE_INCLUDE_DIR "./src/${PROJECT_NAME}")
set(BLARE_CXX_STD 20) # Use C++20
set(BLARE_CMAKE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE
	SOURCE "./src/${PROJECT_NAME}/*.cpp"
)

set(CMAKE_CXX_STANDARD ${BLARE_CXX_STD})

if (BLARE_TESTS)
	enable_testing()
	add_subdirectory(${BLARE_TESTING_DIR})
endif()