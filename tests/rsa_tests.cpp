#include "test_framework.h"

#include "rsa_decrypt.h"
#include "rsa_encrypt.h"

#include <string>
#include <vector>

TEST_CASE("RSA encrypts and decrypts a number") {
    const long long n = 3233;
    const long long e = 17;
    const long long d = 2753;
    const long long message = 65;

    const long long encrypted = RSAEncrypt::encrypt(message, e, n);
    CHECK_EQUAL(RSADecrypt::decrypt(encrypted, d, n), message);
}

TEST_CASE("RSA encrypts and decrypts a string") {
    const long long n = 3233;
    const long long e = 17;
    const long long d = 2753;
    const std::string message = "Test";

    const std::string encrypted = RSAEncrypt::encryptString(message, e, n);
    CHECK_EQUAL(RSADecrypt::decryptString(encrypted, d, n), message);
}

TEST_CASE("RSA decrypts a vector of encrypted numbers") {
    const long long n = 3233;
    const long long e = 17;
    const long long d = 2753;
    const std::string message = "OK";

    std::vector<long long> encrypted;
    for (char ch : message) {
        encrypted.push_back(RSAEncrypt::encrypt(static_cast<unsigned char>(ch), e, n));
    }

    CHECK_EQUAL(RSADecrypt::decryptVector(encrypted, d, n), message);
}
