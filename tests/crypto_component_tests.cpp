#include <gtest/gtest.h>

#include "CryptoComponent.h"
#include "Encryption/rsa_encrypt.h"
#include "Signatures/GOSTSignatureInc.h"

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

using cryptocomponent::CryptoComponent;

// ================== CryptoComponent facade ==================
TEST(CryptoComponentTest, HashFacadeMethods) {
    EXPECT_EQ(CryptoComponent::sha256("abc"), CryptoComponent::hash("abc", "SHA256"));
    EXPECT_EQ(static_cast<size_t>(32), CryptoComponent::stribog256({'a', 'b', 'c'}).size());
    EXPECT_EQ(static_cast<size_t>(64), CryptoComponent::stribog512({'a', 'b', 'c'}).size());
    EXPECT_EQ(static_cast<size_t>(64), CryptoComponent::stribog256Hex("abc").size());
    EXPECT_EQ(static_cast<size_t>(128), CryptoComponent::stribog512Hex("abc").size());
}

TEST(CryptoComponentTest, HashFileFacade) {
    const std::string fileName = "component_hash_file.txt";
    {
        std::ofstream file(fileName, std::ios::binary);
        file << "abc";
    }

    EXPECT_EQ(CryptoComponent::md5("abc"), CryptoComponent::md5File(fileName));
    EXPECT_EQ(CryptoComponent::sha1("abc"), CryptoComponent::hashFile(fileName, "SHA1"));

    std::remove(fileName.c_str());
}

TEST(CryptoComponentTest, HmacFacadeMethods) {
    EXPECT_EQ(
        std::string("f7bc83f430538424b13298e6aa6fb143ef4d59a14946175997479dbc2d1a3cd8"),
        CryptoComponent::hmacSHA256("The quick brown fox jumps over the lazy dog", "key")
    );
    EXPECT_FALSE(CryptoComponent::hmacMD5("data", "key").empty());
    EXPECT_FALSE(CryptoComponent::hmacSHA1("data", "key").empty());
    EXPECT_FALSE(CryptoComponent::hmacSHA512("data", "key").empty());
}

TEST(CryptoComponentTest, SymmetricCiphersFacade) {
    const std::vector<uint8_t> key = {'k', 'e', 'y'};
    const std::vector<uint8_t> data = {'d', 'a', 't', 'a'};

    const std::vector<uint8_t> rc4Encrypted = CryptoComponent::rc4(key, data);
    EXPECT_EQ(rc4Encrypted, CryptoComponent::rc4(key, data));
    EXPECT_NE(data, rc4Encrypted);

    EXPECT_FALSE(CryptoComponent::rc4String("key", "data").empty());
    EXPECT_EQ(std::string("zyx"), CryptoComponent::atbash("abc"));
    EXPECT_EQ(std::string("bcd YZA"), CryptoComponent::caesar("abc XYZ", 1));

    const std::vector<uint8_t> xorEncrypted = CryptoComponent::xorCipher(data, key);
    EXPECT_EQ(data, CryptoComponent::xorCipher(xorEncrypted, key));
    EXPECT_FALSE(CryptoComponent::xorCipherString("data", "key").empty());
}

TEST(CryptoComponentTest, DesAndRsaFacadeMethods) {
    const uint64_t block = 0x0123456789ABCDEFULL;
    const uint64_t key = 0x133457799BBCDFF1ULL;
    const uint64_t encryptedBlock = CryptoComponent::desEncryptBlock(block, key);

    EXPECT_NE(block, encryptedBlock);
    EXPECT_EQ(block, CryptoComponent::desDecryptBlock(encryptedBlock, key));

    const uint64_t n = 3233;
    const uint64_t e = 17;
    const uint64_t d = 2753;
    const uint64_t message = 65;
    const uint64_t encrypted = CryptoComponent::rsaEncrypt(message, e, n);

    EXPECT_EQ(message, CryptoComponent::rsaDecrypt(encrypted, d, n));
    EXPECT_EQ(static_cast<uint64_t>(RSAEncrypt::modPow(4, 13, 497)), CryptoComponent::rsaModPow(4, 13, 497));
    EXPECT_EQ(std::string("Hi"), CryptoComponent::rsaDecryptString(CryptoComponent::rsaEncryptString("Hi", e, n), d, n));
}

TEST(CryptoComponentTest, CadesFacadeMethod) {
    const std::vector<uint8_t> message = {'o', 'k'};
    const std::vector<uint8_t> signature(64, 0x11);
    const std::vector<uint8_t> cades = CryptoComponent::cadesWrap(message, signature);

    EXPECT_FALSE(cades.empty());
    EXPECT_EQ(static_cast<uint8_t>(0x30), cades.front());
}
