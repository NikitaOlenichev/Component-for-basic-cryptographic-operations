#include <gtest/gtest.h>

#include "HashCalculate.h"

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

// ================== HashCalculate ==================
TEST(HashCalculateTest, SHA256StringKnownAnswer) {
    EXPECT_EQ(
        std::string("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"),
        HashCalculate::hashString("abc", "SHA256")
    );
}

TEST(HashCalculateTest, NormalizesAlgorithmName) {
    EXPECT_EQ(
        HashCalculate::hashString("abc", "SHA256"),
        HashCalculate::hashString("abc", "sha-256")
    );
}

TEST(HashCalculateTest, SHA256FileMatchesStringHash) {
    const std::string fileName = "hash_test_input.txt";
    {
        std::ofstream file(fileName, std::ios::binary);
        file << "abc";
    }

    EXPECT_EQ(
        HashCalculate::hashString("abc", "SHA256"),
        HashCalculate::hashFile(fileName, "SHA256")
    );

    std::remove(fileName.c_str());
}

TEST(HashCalculateTest, HmacSHA256KnownAnswer) {
    EXPECT_EQ(
        std::string("f7bc83f430538424b13298e6aa6fb143ef4d59a14946175997479dbc2d1a3cd8"),
        HashCalculate::hmacString("The quick brown fox jumps over the lazy dog", "key", "SHA256")
    );
}

TEST(HashCalculateTest, SupportedAlgorithmsContainsExpectedNames) {
    const std::vector<std::string> algorithms = HashCalculate::supportedAlgorithms();

    EXPECT_NE(algorithms.end(), std::find(algorithms.begin(), algorithms.end(), "SHA256"));
    EXPECT_NE(algorithms.end(), std::find(algorithms.begin(), algorithms.end(), "SHA512"));
    EXPECT_NE(algorithms.end(), std::find(algorithms.begin(), algorithms.end(), "STRIBOG256"));
    EXPECT_NE(algorithms.end(), std::find(algorithms.begin(), algorithms.end(), "STRIBOG512"));
}

TEST(HashCalculateTest, StribogHashesHaveExpectedHexLengths) {
    EXPECT_EQ(static_cast<size_t>(64), HashCalculate::hashString("abc", "stribog-256").size());
    EXPECT_EQ(static_cast<size_t>(128), HashCalculate::hashString("abc", "stribog-512").size());
}
