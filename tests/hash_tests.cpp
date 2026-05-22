#include "test_framework.h"

#include "HashCalculate.h"

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <string>

TEST_CASE("HashCalculate computes SHA256 for a string") {
    CHECK_EQUAL(
        HashCalculate::hashString("abc", "SHA256"),
        std::string("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad")
    );
}

TEST_CASE("HashCalculate normalizes algorithm names") {
    CHECK_EQUAL(
        HashCalculate::hashString("abc", "sha-256"),
        HashCalculate::hashString("abc", "SHA256")
    );
}

TEST_CASE("HashCalculate computes SHA256 for a file") {
    const std::string fileName = "hash_test_input.txt";
    {
        std::ofstream file(fileName, std::ios::binary);
        file << "abc";
    }

    CHECK_EQUAL(
        HashCalculate::hashFile(fileName, "SHA256"),
        HashCalculate::hashString("abc", "SHA256")
    );

    std::remove(fileName.c_str());
}

TEST_CASE("HashCalculate computes HMAC SHA256") {
    CHECK_EQUAL(
        HashCalculate::hmacString("The quick brown fox jumps over the lazy dog", "key", "SHA256"),
        std::string("f7bc83f430538424b13298e6aa6fb143ef4d59a14946175997479dbc2d1a3cd8")
    );
}

TEST_CASE("HashCalculate exposes supported algorithms") {
    const std::vector<std::string> algorithms = HashCalculate::supportedAlgorithms();

    CHECK(std::find(algorithms.begin(), algorithms.end(), "SHA256") != algorithms.end());
    CHECK(std::find(algorithms.begin(), algorithms.end(), "SHA512") != algorithms.end());
    CHECK(std::find(algorithms.begin(), algorithms.end(), "STRIBOG256") != algorithms.end());
    CHECK(std::find(algorithms.begin(), algorithms.end(), "STRIBOG512") != algorithms.end());
}

TEST_CASE("HashCalculate computes Stribog hashes with expected hex lengths") {
    CHECK_EQUAL(HashCalculate::hashString("abc", "stribog-256").size(), static_cast<size_t>(64));
    CHECK_EQUAL(HashCalculate::hashString("abc", "stribog-512").size(), static_cast<size_t>(128));
}
