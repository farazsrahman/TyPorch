cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 11)

project(neural)

add_library(${PROJECT_NAME} layers.cpp  models.cpp activation.cpp)