#include <gtest/gtest.h>

#include "Decryption/rc4_decrypt.h"
#include "Encryption/rc4_encrypt.h"
#include "Encryption/xorsh.h"

#include <cstdio>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

// ================== RC4 ==================
TEST(Rc4Test, StringRoundTrip) {
    const std::string key = "secret";
    const std::string plain = "Hello, RC4!";

    RC4_Encryption encryptor(key);
    const std::vector<unsigned char> encrypted = encryptor.encrypt(plain);

    RC4_Decryption decryptor(key);
    const std::string recovered = decryptor.decryptString(
        std::string(encrypted.begin(), encrypted.end())
    );

    EXPECT_EQ(plain, recovered);
}

TEST(Rc4Test, BytesRoundTrip) {
    const std::string key = "key456";
    const std::vector<unsigned char> plain = {0x01, 0x02, 0x03, 0x04, 0x05};

    RC4_Encryption encryptor(key);
    const std::vector<unsigned char> encrypted = encryptor.encrypt(plain);

    RC4_Decryption decryptor(key);
    EXPECT_EQ(plain, decryptor.decryptBytes(encrypted));
}

TEST(Rc4Test, EmptyKeyFallsBackToDefaultKey) {
    const std::vector<unsigned char> data = {0x10, 0x20, 0x30};
    RC4_Encryption encryptor(std::vector<unsigned char>{});

    const std::vector<unsigned char> first = encryptor.encrypt(data);
    const std::vector<unsigned char> second = encryptor.encrypt(data);

    EXPECT_EQ(first, second);
    EXPECT_NE(data, first);
}

TEST(Rc4Test, ResetAndSetKeyKeepEncryptionDeterministic) {
    RC4_Encryption encryptor;
    const std::vector<unsigned char> data = {0x01, 0x02, 0x03};

    encryptor.setKey("test");
    const std::vector<unsigned char> first = encryptor.encrypt(data);
    encryptor.reset();
    const std::vector<unsigned char> second = encryptor.encrypt(data);
    encryptor.setKey(std::vector<unsigned char>{'t', 'e', 's', 't'});
    const std::vector<unsigned char> third = encryptor.encrypt("abc");

    EXPECT_EQ(first, second);
    EXPECT_FALSE(third.empty());
}

TEST(Rc4Test, FileRoundTrip) {
    const std::string key = "filekey";
    const std::string encryptedFile = "rc4_test_encrypted.bin";
    const std::string outputFile = "rc4_test_decrypted.bin";
    const std::vector<unsigned char> original = {0x10, 0x20, 0x30, 0x40};

    RC4_Encryption encryptor(key);
    const std::vector<unsigned char> encrypted = encryptor.encrypt(original);
    {
        std::ofstream out(encryptedFile, std::ios::binary);
        out.write(reinterpret_cast<const char*>(encrypted.data()), encrypted.size());
    }

    RC4_Decryption decryptor(key);
    decryptor.decryptFile(encryptedFile, outputFile);

    std::ifstream in(outputFile, std::ios::binary);
    const std::vector<unsigned char> recovered{
        std::istreambuf_iterator<char>(in),
        std::istreambuf_iterator<char>()
    };

    EXPECT_EQ(original, recovered);

    std::remove(encryptedFile.c_str());
    std::remove(outputFile.c_str());
}

// ================== Base64 / XOR ==================
TEST(XorBase64Test, Base64EncodeDecode) {
    const std::string text = "any carnal pleasure.";
    const std::string encoded = base64_encode(text);

    EXPECT_NE(std::string::npos, encoded.find('='));
    EXPECT_EQ(text, base64_decode(encoded));
}

TEST(XorBase64Test, XorCryptIsReversible) {
    const std::string text = "Secret text";
    const std::string key = "key";
    const std::string encrypted = xorCrypt(text, key);

    EXPECT_NE(text, encrypted);
    EXPECT_EQ(text, xorCrypt(encrypted, key));
}

TEST(XorBase64Test, FileRoundTrip) {
    const std::string fileName = "xor_file_test.txt";
    const std::string encryptedFile = fileName + ".enc";
    const std::string text = "File content";

    {
        std::ofstream out(fileName, std::ios::binary);
        out << text;
    }

    encryptFile(fileName, "key");
    {
        std::ofstream out(fileName, std::ios::binary);
        out << "changed";
    }
    decryptFile(encryptedFile, "key");

    std::ifstream in(fileName, std::ios::binary);
    const std::string recovered{
        std::istreambuf_iterator<char>(in),
        std::istreambuf_iterator<char>()
    };

    EXPECT_EQ(text, recovered);

    std::remove(fileName.c_str());
    std::remove(encryptedFile.c_str());
}
