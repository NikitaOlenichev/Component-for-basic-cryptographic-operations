cmake_minimum_required(VERSION 3.15)
project(Component_for_basic_cryptographic_operations VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(MSVC)
    add_compile_options(/utf-8)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

option(BUILD_SHARED_LIBS "Build as shared library" ON)
option(CPPCRYPTO_BUILD_TESTS "Build unit tests" OFF)
option(CPPCRYPTO_BUILD_EXAMPLES "Build examples" ON)
option(CPPCRYPTO_ENABLE_COVERAGE "Build with GCC coverage flags" OFF)

add_compile_definitions(CRYPTOCOMPONENT_BUILD_DLL)

# Поиск OpenSSL
find_package(OpenSSL QUIET)
set(USE_OPENSSL ${OPENSSL_FOUND})
if(USE_OPENSSL)
    message(STATUS "OpenSSL found, enabling hashing functions")
else()
    message(STATUS "OpenSSL not found, hashing (MD5, SHA) will be disabled")
endif()

# Основная библиотека
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
        src/Signatures/BigInteger.cpp
        src/Signatures/CAdESBuilder.cpp
        src/Signatures/SignatureParams.cpp
        src/Signatures/Stribog.cpp
        src/CryptoComponent.cpp
)

# Хеш-функции добавляются только если установлен OpenSSL
if(USE_OPENSSL)
    target_sources(Component_for_basic_cryptographic_operations PRIVATE src/Hash/HashCalculate.cpp)
    target_link_libraries(Component_for_basic_cryptographic_operations PRIVATE OpenSSL::Crypto)
endif()

target_include_directories(Component_for_basic_cryptographic_operations PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(CPPCRYPTO_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(Component_for_basic_cryptographic_operations PRIVATE -O0 -g --coverage)
    target_link_options(Component_for_basic_cryptographic_operations PRIVATE --coverage)
endif()

# Установка
include(GNUInstallDirs)
install(TARGETS Component_for_basic_cryptographic_operations
        EXPORT Component_for_basic_cryptographic_operationsTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Component_for_basic_cryptographic_operations)

install(EXPORT Component_for_basic_cryptographic_operationsTargets
        FILE Component_for_basic_cryptographic_operationsTargets.cmake
        NAMESPACE Component_for_basic_cryptographic_operations::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Component_for_basic_cryptographic_operations
)

# Примеры
if(CPPCRYPTO_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Тесты
if(CPPCRYPTO_BUILD_TESTS)
    enable_testing()

    add_executable(unit_tests
            tests/test_main.cpp
            tests/hash_tests.cpp
            tests/rsa_tests.cpp
            tests/des_tests.cpp
            tests/atbash_tests.cpp
            tests/rc4_xor_tests.cpp
            tests/signature_tests.cpp
            tests/crypto_component_tests.cpp
    )

    target_include_directories(unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    target_link_libraries(unit_tests PRIVATE Component_for_basic_cryptographic_operations)

    if(CPPCRYPTO_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(unit_tests PRIVATE -O0 -g --coverage)
        target_link_options(unit_tests PRIVATE --coverage)
    endif()

    add_test(NAME unit_tests COMMAND unit_tests)
endif()
