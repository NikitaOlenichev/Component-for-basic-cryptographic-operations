#include <gtest/gtest.h>

#include "Decryption/rsa_decrypt.h"
#include "Encryption/rsa_encrypt.h"

#include <string>
#include <vector>

// ================== RSA ==================
TEST(RsaTest, EncryptDecryptNumber) {
    const long long n = 3233;
    const long long e = 17;
    const long long d = 2753;
    const long long message = 65;

    const long long encrypted = RSAEncrypt::encrypt(message, e, n);
    EXPECT_EQ(message, RSADecrypt::decrypt(encrypted, d, n));
}

TEST(RsaTest, EncryptDecryptString) {
    const long long n = 3233;
    const long long e = 17;
    const long long d = 2753;
    const std::string message = "Test";

    const std::string encrypted = RSAEncrypt::encryptString(message, e, n);
    EXPECT_EQ(message, RSADecrypt::decryptString(encrypted, d, n));
}

TEST(RsaTest, DecryptVector) {
    const long long n = 3233;
    const long long e = 17;
    const long long d = 2753;
    const std::string message = "OK";

    std::vector<long long> encrypted;
    for (char ch : message) {
        encrypted.push_back(RSAEncrypt::encrypt(static_cast<unsigned char>(ch), e, n));
    }

    EXPECT_EQ(message, RSADecrypt::decryptVector(encrypted, d, n));
}

TEST(RsaTest, EncryptDecryptHexString) {
    const long long n = 3233;
    const long long e = 17;
    const long long d = 2753;
    const std::string message = "Hex";

    const std::string encrypted = RSAEncrypt::encryptStringHex(message, e, n);
    EXPECT_EQ(message, RSADecrypt::decryptStringHex(encrypted, d, n));
}

TEST(RsaTest, InvalidDecryptedCharacterIsReplaced) {
    const long long n = 3233;
    const long long d = 1;

    EXPECT_EQ(std::string("?"), RSADecrypt::decryptString("300 ", d, n));
    EXPECT_EQ(std::string("?"), RSADecrypt::decryptStringHex("0x12c ", d, n));
}
