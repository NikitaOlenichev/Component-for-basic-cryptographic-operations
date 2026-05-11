cmake_minimum_required(VERSION 3.31)
project(Component_for_basic_cryptographic_operations)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(Component_for_basic_cryptographic_operations
        src/Decription/des.cpp
        src/Decription/rc4.cpp
        src/Decription/rsa_decrypt.cpp
        src/Encription/CesarCypher.cpp
        src/Encription/DEScypher.cpp
        src/Encription/rc4.cpp
        src/Encription/rsa_encrypt.cpp
        src/Encription/shifratbasha.cpp
        src/Encription/xorsh.cpp
        src/Hash/HashCalculate.cpp
        src/Signatures/BigInteger.cpp
        src/Signatures/CAdESBuilder.cpp
        src/Signatures/SignatureParams.cpp
        src/Signatures/Stribog.cpp
        tests/test1.cpp)
