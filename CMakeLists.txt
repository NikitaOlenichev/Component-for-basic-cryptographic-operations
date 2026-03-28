cmake_minimum_required(VERSION 3.31)
project(Component_for_basic_cryptographic_operations)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(Component_for_basic_cryptographic_operations
        include/1.h
        src/1.cpp
        UnitTests/UnitTest1.cpp
        main.cpp)
