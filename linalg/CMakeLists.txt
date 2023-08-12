cmake_minimum_required(VERSION 3.13)
project(linalg)

# Set the C++ compiler to g++
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 11)

# # Check for the -MP flag
# option(USE_MP "Compile tensorMP.cpp instead of tensor.cpp" OFF)

# # Define the sources based on the flag
# if(USE_MP)
#     set(SOURCES tensorMP.cpp matrix.cpp)
# else()
#     set(SOURCES tensor.cpp matrix.cpp)
# endif()


# Create the library
add_library(${PROJECT_NAME} matrixMP.cpp tensorMP.cpp)