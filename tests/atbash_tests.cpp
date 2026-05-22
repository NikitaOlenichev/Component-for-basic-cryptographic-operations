#include "test_framework.h"

#include "shifratbasha.h"

#include <string>

TEST_CASE("Atbash encrypts latin letters") {
    shifratbasha cipher;

    CHECK_EQUAL(cipher.encrypt("abc XYZ"), std::string("zyx CBA"));
}

TEST_CASE("Atbash decrypt reverses encryption") {
    shifratbasha cipher;
    const std::string text = "Hello, World!";

    CHECK_EQUAL(cipher.decrypt(cipher.encrypt(text)), text);
}
