cmake_minimum_required(VERSION 3.31)
project(Component_for_basic_cryptographic_operations)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)

set(PROJECT_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Decription
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Encription
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Hash
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Signatures)

add_library(crypto_component STATIC
        src/Hash/HashCalculate.cpp
        src/Signatures/Stribog.cpp
        src/Encription/DEScypher.cpp
        src/Encription/rsa_encrypt.cpp
        src/Decription/rsa_decrypt.cpp
        src/Encription/shifratbasha.cpp)

target_include_directories(crypto_component PUBLIC ${PROJECT_INCLUDE_DIRS})
target_compile_definitions(crypto_component PRIVATE CRYPTO_COMPONENT_LIBRARY)
target_link_libraries(crypto_component PUBLIC OpenSSL::Crypto)

enable_testing()

add_executable(unit_tests
        tests/test_main.cpp
        tests/hash_tests.cpp
        tests/rsa_tests.cpp
        tests/des_tests.cpp
        tests/atbash_tests.cpp)

target_include_directories(unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
target_link_libraries(unit_tests PRIVATE crypto_component)

add_test(NAME unit_tests COMMAND unit_tests)
