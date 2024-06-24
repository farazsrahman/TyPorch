cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX13.sdk)


project(neural)


add_library(${PROJECT_NAME} layers.cpp  models.cpp)

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../linalg/build)
target_link_libraries(${PROJECT_NAME} PUBLIC linalg)
