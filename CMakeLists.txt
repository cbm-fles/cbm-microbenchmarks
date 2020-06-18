cmake_minimum_required(VERSION 3.10)
project(cbm-microbenchmarks)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-msse4.2 -Wall -Wpedantic -Wextra -Winit-self -Wundef -Wold-style-cast -Woverloaded-virtual -Wwrite-strings -Wnon-virtual-dtor -fno-omit-frame-pointer)

add_executable(return_vector return_vector.cpp Stopwatch.h)
add_executable(reader_interface reader_interface.cpp Stopwatch.h)
