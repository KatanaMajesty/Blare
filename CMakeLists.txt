cmake_minimum_required(VERSION 3.23.2 FATAL_ERROR)
project(blare VERSION 0.1.0 LANGUAGES CXX C)

set(BLARE_TESTS ON)
set(BLARE_TESTING_DIR "blare-testing")
set(BLARE_INCLUDE_DIR "./src/${PROJECT_NAME}")

file(GLOB_RECURSE
	SOURCE "./src/${PROJECT_NAME}/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCE}) # temporary executable
target_include_directories(${PROJECT_NAME}
	PUBLIC ${BLARE_INCLUDE_DIR}
)

if (BLARE_TESTS)
	enable_testing()
	add_subdirectory(${BLARE_TESTING_DIR})
endif()