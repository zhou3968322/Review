cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(radix_sort LANGUAGES CXX CUDA)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 11)

# Create an executable with both C++ and CUDA sources
add_executable(radix_sort_cpu radix_sort.cpp)