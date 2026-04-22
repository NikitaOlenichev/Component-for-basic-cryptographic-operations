//
// Реализует хеш-функцию по ГОСТ Р 34.11-2012 (Стрибог),
// которая преобразует сообщение произвольной длины в хеш-код фиксированного размера (256 или 512 бит).
// Copyright (c) Andreyinthesky. All rights reserved.
// (https://github.com/Andreyinthesky/cpp-cryptography/blob/master/algorithms/gost_stribog.cpp).
//

#include "Stribog.h"
#include <cstring>
#include <algorithm>
#include <stdexcept>

const uint8_t Stribog::Pi[256] = {
    252, 238, 221, 17, 207, 110, 49, 22, 251, 196, 250, 218, 35, 197, 4, 77,
    233, 119, 240, 219, 147, 46, 153, 186, 23, 54, 241, 187, 20, 205, 95, 193,
    249, 24, 101, 90, 226, 92, 239, 33, 129, 28, 60, 66, 139, 1, 142, 79,
    5, 132, 2, 174, 227, 106, 143, 160, 6, 11, 237, 152, 127, 212, 211, 31,
    235, 52, 44, 81, 234, 200, 72, 171, 242, 42, 104, 162, 253, 58, 206, 204,
    181, 112, 14, 86, 8, 12, 118, 18, 191, 114, 19, 71, 156, 183, 93, 135,
    21, 161, 150, 41, 16, 123, 154, 199, 243, 145, 120, 111, 157, 158, 178, 177,
    50, 117, 25, 61, 255, 53, 138, 126, 109, 84, 198, 128, 195, 189, 13, 87,
    223, 245, 36, 169, 62, 168, 67, 201, 215, 121, 214, 246, 124, 34, 185, 3,
    224, 15, 236, 222, 122, 148, 176, 188, 220, 232, 40, 80, 78, 51, 10, 74,
    167, 151, 96, 115, 30, 0, 98, 68, 26, 184, 56, 130, 100, 159, 38, 65,
    173, 69, 70, 146, 39, 94, 85, 47, 140, 163, 165, 125, 105, 213, 149, 59,
    7, 88, 179, 64, 134, 172, 29, 247, 48, 55, 107, 228, 136, 217, 231, 137,
    225, 27, 131, 73, 76, 63, 248, 254, 141, 83, 170, 144, 202, 216, 133, 97,
    32, 113, 103, 164, 45, 43, 9, 91, 203, 155, 37, 208, 190, 229, 108, 82,
    89, 166, 116, 210, 230, 244, 180, 192, 209, 102, 175, 194, 57, 75, 99, 182
};

const int Stribog::Tau[64] = {
    0,  8, 16, 24, 32, 40, 48, 56,
    1,  9, 17, 25, 33, 41, 49, 57,
    2, 10, 18, 26, 34, 42, 50, 58,
    3, 11, 19, 27, 35, 43, 51, 59,
    4, 12, 20, 28, 36, 44, 52, 60,
    5, 13, 21, 29, 37, 45, 53, 61,
    6, 14, 22, 30, 38, 46, 54, 62,
    7, 15, 23, 31, 39, 47, 55, 63
};

const uint64_t Stribog::IV_256[8] = {
    0x6b68c3b3, 0x95b1d092, 0x8c31d1b9, 0x843faa51,
    0xcd9b5294, 0x490f5c3d, 0x29e5a48d, 0x10d6e834
};

const uint64_t Stribog::IV_512[8] = {
    0x47d41f5b, 0x64531346, 0xa9518d5d, 0xc4cd95b1,
    0x539b2e9e, 0xae542d49, 0x7142740f, 0xeb0b2ac6
};

const uint64_t Stribog::C[12][8] = {
    {0xb1085bda1ecadae9, 0xe5f7c3eb0c63cee4, 0x53fbc81a09cbd364, 0x2eabe5fd32c2d59c,
     0x99ba33be6e59ffc8, 0xb8f1e8bc9eaa83c7, 0x79e274be5dcce20d, 0xddbabc92fcf5cefa},
    {0xbd5840080bccc98e, 0xcd0a717fc2bd9c7c, 0x3582572d59343494, 0x155f4bafaa680707,
     0x118cf6ab99e7d488, 0xea00e946c1bd6a36, 0x2d1fc5466e00351e, 0xfa20668a9e45f9bc},
    {0x65e3962ebf52e3d1, 0x7dd891b06d17f2ad, 0x5f2b00cdfb5470aa, 0x9e847afa29a13304,
     0xe49d3e0dacb956e0, 0xeb7f533f951b89d0, 0x099b877d6185813c, 0x5a71dbe5b6b927b2},
    {0x971ab6f9b255b6ca, 0x6e03ddbfb19522c4, 0xf270b577c299a96a, 0x6f5958f269af5159,
     0x045456089353b9f4, 0x03137c6638e1da48, 0x09ce3449c25dd672, 0x1b8a18a4b129d1c0},
    {0xaef4353bf091eb21, 0xa26a48e7da20efae, 0x0bcf0dcc776c4ca9, 0x2cacd47c98587b02,
     0x31f674d3f6f178dd, 0xc961afa68861eaa5, 0x5601ea3cc2260e41, 0x22bce5a76b690f43},
    {0x2043be55c216f9f7, 0x58bcaad89b1081ad, 0x153319fa9011cbbd, 0x1f815d6c32f6e9b6,
     0x6076cddd955e2407, 0xc5bd2b1fa4b7aedd, 0xd92d53a9836b21cb, 0xed71b0cc202fa5de},
    {0x49b3245a135bda73, 0x4e179f18bd6cc029, 0x35fe21acda750a2b, 0xab4b6f6a36b22cbf,
     0x824bfa4b06c9fccb, 0xfa1c759a1a234ffc, 0xa4a6d9b14e1a01e3, 0x472faadf10b8c901},
    {0x2d65a1495e02536b, 0x1096bb09a942a21e, 0xa69dd1f115f10b2c, 0x137c673a52d02a2f,
     0x50eb101c43c3c5c3, 0x8fcc9a11e7ab9f2f, 0x8fdda95484a26dfc, 0x3519d1b220d3ffa5},
    {0x167a595c0da8f3c3, 0xada4746e9ec07373, 0xf11bdd9b5bd5a04d, 0xed7483f4c333d4ca,
     0x1760b911f93faf6a, 0xf99bc5691e4e1faa, 0xdf586d7fad09eacc, 0x172b9c0176e2f946},
    {0x1ca195a67363e6f4, 0x56ac9e637ababd9c, 0x83e04b444b4bbad4, 0x98f67e0e5a24db98,
     0x2d5c0a7f5a64b1b2, 0xa2cf3351667ba2e1, 0x6f11cbae724b8403, 0xfb5f5af449791ba0},
    {0x1eecb713e7ddb1c1, 0x179ccf455e445612, 0xad198bb1b2ca4bde, 0x40428bbbecb90b1c,
     0x12e790d8ce4aa48a, 0xb1f98d64af442ef3, 0x6b751a6ef4a5e4d2, 0x8bda2f64bebfafb3},
    {0x76482f4f61d74b13, 0x1e2c27b9ab119281, 0xb1d54b0efba8c9c6, 0x2e53cbedc2b0bcf1,
     0x14a611153f0b10a7, 0xc666cd150d7cf59f, 0xb28c86c1c482f6aa, 0x534effcda3f5c55b}
};

Stribog::Stribog(HashLength hash_len) : hash_len(hash_len) {}

std::vector<uint8_t> Stribog::calculate(const std::vector<uint8_t>& msg) {
    this->message = msg;
    padding();

    uint64_t h[8];
    uint64_t N[8] = {0};
    uint64_t Sigma[8] = {0};
    const uint64_t* iv = (hash_len == b256) ? IV_256 : IV_512;
    memcpy(h, iv, 64);

    size_t blocks = message.size() / 64;
    for (size_t i = 0; i < blocks; ++i) {
        processBlock(h, N, Sigma, message.data() + i * 64);
    }

    std::vector<uint8_t> result(hash_len == b256 ? 32 : 64);
    for (int i = 0; i < (hash_len == b256 ? 32 : 64); ++i) {
        result[i] = (h[i / 8] >> (8 * (i % 8))) & 0xFF;
    }
    return result;
}

void Stribog::padding() {
    uint64_t msg_len_bits = message.size() * 8;
    message.push_back(0x01);
    while ((message.size() % 64) != 56) {
        message.push_back(0x00);
    }
    for (int i = 0; i < 8; ++i) {
        message.push_back((msg_len_bits >> (i * 8)) & 0xFF);
    }
}

void Stribog::processBlock(uint64_t* h, uint64_t* N, uint64_t* Sigma, const uint8_t* block) {
    uint64_t m[8];
    for (int i = 0; i < 8; ++i) {
        m[i] = 0;
        for (int j = 0; j < 8; ++j) {
            m[i] |= (uint64_t)block[i*8 + j] << (j*8);
        }
    }

    uint64_t h_new[8];
    memcpy(h_new, h, 64);
    g(h_new, N, m);

    addMod512(N, m);
    addMod512(Sigma, h_new);

    memcpy(h, h_new, 64);
}

void Stribog::lps(uint8_t* block) {
    for (int i = 0; i < 64; ++i) {
        block[i] = Pi[block[i]];
    }
    uint8_t temp[64];
    memcpy(temp, block, 64);
    for (int i = 0; i < 64; ++i) {
        block[i] = temp[Tau[i]];
    }
    uint64_t x[8], y[8];
    for (int i = 0; i < 8; ++i) {
        x[i] = 0;
        for (int j = 0; j < 8; ++j) {
            x[i] |= (uint64_t)block[i*8 + j] << (j*8);
        }
    }
    for (int i = 0; i < 8; ++i) {
        y[i] = x[i] ^ (x[(i+1)%8] >> 1) ^ (x[(i+2)%8] >> 2) ^ (x[(i+3)%8] >> 3);
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            block[i*8 + j] = (y[i] >> (j*8)) & 0xFF;
        }
    }
}

void Stribog::g(uint64_t* h, const uint64_t* N, const uint64_t* m) {
    uint64_t K[8];
    memcpy(K, h, 64);
    for (int i = 0; i < 12; ++i) {
        uint64_t temp[8];
        xor512(temp, K, C[i]);
        e(temp, m);
        xor512(K, temp, K);
        xor512(K, K, C[i]);
    }
    xor512(h, K, h);
    xor512(h, h, m);
}

void Stribog::e(uint64_t* K, const uint64_t* m) {
    uint64_t state[8];
    memcpy(state, m, 64);
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 8; ++j) state[j] ^= K[j];
        // LPS
        uint8_t block[64];
        for (int j = 0; j < 8; ++j) {
            for (int k = 0; k < 8; ++k) {
                block[j*8 + k] = (state[j] >> (k*8)) & 0xFF;
            }
        }
        lps(block);
        for (int j = 0; j < 8; ++j) {
            state[j] = 0;
            for (int k = 0; k < 8; ++k) {
                state[j] |= (uint64_t)block[j*8 + k] << (k*8);
            }
        }
        for (int j = 0; j < 8; ++j) K[j] ^= C[i][j];
    }
    memcpy(K, state, 64);
}

void Stribog::addMod512(uint64_t* a, const uint64_t* b) {
    uint64_t carry = 0;
    for (int i = 0; i < 8; ++i) {
        uint64_t sum = a[i] + b[i] + carry;
        carry = (sum < a[i] || (carry && sum == a[i])) ? 1 : 0;
        a[i] = sum;
    }
}

void Stribog::xor512(uint64_t* result, const uint64_t* a, const uint64_t* b) {
    for (int i = 0; i < 8; ++i) result[i] = a[i] ^ b[i];
}

std::vector<uint8_t> Stribog::hash256(const std::vector<uint8_t>& msg) {
    Stribog st(b256);
    return st.calculate(msg);
}

std::vector<uint8_t> Stribog::hash512(const std::vector<uint8_t>& msg) {
    Stribog st(b512);
    return st.calculate(msg);
}
