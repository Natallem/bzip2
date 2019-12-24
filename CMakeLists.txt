cmake_minimum_required(VERSION 3.4)
project(bzip2_with_tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SOURCE_FILES main.cpp)
add_executable(bzip2_run ${SOURCE_FILES})


include_directories(bzip2_lib)
add_subdirectory(bzip2_lib)

target_link_libraries(bzip2_run bzip2_lib)
add_subdirectory(google_tests)
add_executable(bzip2 ${SOURCE_FILES} main.cpp)

