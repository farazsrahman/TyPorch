cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 11)

project(neural)


add_library(${PROJECT_NAME} layers.cpp  models.cpp)

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../linalg/build)
target_link_libraries(${PROJECT_NAME} PUBLIC linalg)
