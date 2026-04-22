cmake_minimum_required(VERSION 3.31)
project(Component_for_basic_cryptographic_operations)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Component_for_basic_cryptographic_operations
        BigInteger.cpp
        CAdESBuilder.cpp
        EllipticPoint.cpp
        main.cpp
        Stribog.cpp
        SignatureParams.cpp)
