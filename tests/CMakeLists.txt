cmake_minimum_required(VERSION 3.14)
project(containers_test)

set(CMAKE_CXX_STANDARD 98)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    stl_test
    vector_test.cpp
    stack_test.cpp
    map_test.cpp
    set_test.cpp
    ../tree/tree.cpp
)

target_link_libraries(
    stl_test
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(stl_test)
