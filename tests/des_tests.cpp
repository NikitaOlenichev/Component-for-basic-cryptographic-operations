#include <gtest/gtest.h>

#include "DEScypher.h"

#include <cstdint>
#include <string>

// ================== DES (encrypt) ==================
TEST(DesEncryptTest, EncryptionIsDeterministic) {
    const uint64_t plaintext = 0x0123456789ABCDEFULL;
    const uint64_t key = 0x133457799BBCDFF1ULL;

    EXPECT_EQ(DEScypher::encrypt(plaintext, key), DEScypher::encrypt(plaintext, key));
    EXPECT_NE(plaintext, DEScypher::encrypt(plaintext, key));
}

TEST(DesEncryptTest, StringToBlockAndBack) {
    EXPECT_EQ(0x4142434445464748ULL, DEScypher::stringToBlock("ABCDEFGH"));
    EXPECT_EQ(std::string("ABCDEFGH"), DEScypher::blockToString(0x4142434445464748ULL));
}
