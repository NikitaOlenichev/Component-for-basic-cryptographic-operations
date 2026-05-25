cmake_minimum_required(VERSION 3.31)
project(Component_for_basic_cryptographic_operations VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(BUILD_SHARED_LIBS "Build as shared library" ON)
option(CPPCRYPTO_BUILD_TESTS "Build unit tests" OFF) # ON

add_compile_definitions(CRYPTOCOMPONENT_BUILD_DLL)

#find_package(OpenSSL REQUIRED)

add_library(Component_for_basic_cryptographic_operations
        src/Decryption/des.cpp
        src/Decryption/rc4_decrypt.cpp
        src/Decryption/rsa_decrypt.cpp
        src/Encryption/CesarCypher.cpp
        src/Encryption/DEScypher.cpp
        src/Encryption/rc4_encrypt.cpp
        src/Encryption/rsa_encrypt.cpp
        src/Encryption/shifratbasha.cpp
        src/Encryption/xorsh.cpp
        #src/Hash/HashCalculate.cpp
        src/Signatures/BigInteger.cpp
        src/Signatures/CAdESBuilder.cpp
        src/Signatures/SignatureParams.cpp
        src/Signatures/Stribog.cpp
        src/CryptoComponent.cpp
)

target_include_directories(Component_for_basic_cryptographic_operations PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

#target_link_libraries(Component_for_basic_cryptographic_operations PRIVATE OpenSSL::Crypto)

include(GNUInstallDirs)
install(TARGETS Component_for_basic_cryptographic_operations
        EXPORT Component_for_basic_cryptographic_operationsTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cppcrypto)

install(EXPORT Component_for_basic_cryptographic_operationsTargets
        FILE Component_for_basic_cryptographic_operationsTargets.cmake
        NAMESPACE Component_for_basic_cryptographic_operations::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Component_for_basic_cryptographic_operations
)

add_subdirectory(examples)

if(CPPCRYPTO_BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)
    add_subdirectory(tests)
endif()
