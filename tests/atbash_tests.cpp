#include <gtest/gtest.h>

#include "shifratbasha.h"

#include <string>

// ================== shifratbasha (Atbash) ==================
TEST(ShifratbashaTest, EncryptLatinLetters) {
    shifratbasha cipher;

    EXPECT_EQ(std::string("zyx CBA"), cipher.encrypt("abc XYZ"));
}

TEST(ShifratbashaTest, EncryptDecrypt) {
    shifratbasha cipher;
    const std::string text = "Hello, World!";

    EXPECT_EQ(text, cipher.decrypt(cipher.encrypt(text)));
}
