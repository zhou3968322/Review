# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.10)

# Project's name
project(TestFloatMath)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Can manually add the sources using the set command as follows:
# set(SOURCES src/main.cpp)


# Add library
file(GLOB_RECURSE LIB_SOURCES "src/*.cpp")
add_library(float_arithmetic STATIC ${LIB_SOURCES})

# Add executable
add_executable(TestFloatMath test/main.cpp)
target_link_libraries(TestFloatMath float_arithmetic)

# If you have any libraries to link (e.g., pthreads, boost, etc.):
# target_link_libraries(my_project library_name)