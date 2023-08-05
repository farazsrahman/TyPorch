#linalg library build script
cmake_minimum_required(VERSION 3.13)
project(neuraltest)

# min requirements for GTEST
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# fetching the GTest library straight from GitHub
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip 
) # the above hash may need to be updated time to time ^^
FetchContent_MakeAvailable(googletest)

enable_testing()

# main executable for testing
add_executable(${PROJECT_NAME} test.cpp)

# specifying where to look for directories
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../build ${CMAKE_CURRENT_SOURCE_DIR}/../../linalg/build)

target_link_libraries(${PROJECT_NAME}

    PRIVATE
        neural
        linalg

    PUBLIC
        GTest::gtest_main

)

# Add the include directories for Google Test
target_include_directories(${PROJECT_NAME} PRIVATE ${googletest_SOURCE_DIR}/googletest/include)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})