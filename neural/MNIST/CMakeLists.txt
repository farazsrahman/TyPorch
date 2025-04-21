#linalg library build script
cmake_minimum_required(VERSION 3.13)
project(mnist_train)

# min requirements for GTEST
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# main executable for testing
add_executable(${PROJECT_NAME} train_mnist.cpp)

# specifying where to look for directories
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../build ${CMAKE_CURRENT_SOURCE_DIR}/../../linalg/build)

target_link_libraries(${PROJECT_NAME}

    PRIVATE
        neural
        linalg

)
