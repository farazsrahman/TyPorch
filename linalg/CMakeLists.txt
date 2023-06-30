#linalg library build script
cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 11)

project(linalg)

add_library(${PROJECT_NAME} matrix.cpp  tensor.cpp)