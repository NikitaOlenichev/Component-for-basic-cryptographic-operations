#include "test_framework.h"

#include "DEScypher.h"

#include <cstdint>
#include <string>

TEST_CASE("DES encryption is deterministic") {
    const uint64_t plaintext = 0x0123456789ABCDEFULL;
    const uint64_t key = 0x133457799BBCDFF1ULL;

    CHECK_EQUAL(DEScypher::encrypt(plaintext, key), DEScypher::encrypt(plaintext, key));
    CHECK(DEScypher::encrypt(plaintext, key) != plaintext);
}

TEST_CASE("DES converts strings to fixed-size blocks") {
    CHECK_EQUAL(DEScypher::stringToBlock("ABCDEFGH"), 0x4142434445464748ULL);
    CHECK_EQUAL(DEScypher::blockToString(0x4142434445464748ULL), std::string("ABCDEFGH"));
}
