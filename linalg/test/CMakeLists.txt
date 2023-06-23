cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 11)

project(testlinalg)

add_executable(${PROJECT_NAME} test.cpp)

target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/../build)

target_link_libraries(${PROJECT_NAME} linalg)
