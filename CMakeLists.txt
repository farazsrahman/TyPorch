cmake_minimum_required(VERSION 3.13)
project(linalgtest)

set(CMAKE_CXX_STANDARD 11)

include(CTest) # Include CMake's support for testing functionality
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../external) # Include external directory

if(BUILD_TESTNG)
    find_package(GTest REQUIRED)
    enable_testing()

    add_executable(${PROJECT_NAME} test.cpp)
    target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main linalg) # Link with GTest and linalg libraries

    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME})
endif()