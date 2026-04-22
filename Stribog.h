//
// Реализует хеш-функцию по ГОСТ Р 34.11-2012 (Стрибог),
// которая преобразует сообщение произвольной длины в хеш-код фиксированного размера (256 или 512 бит).
// Copyright (c) Andreyinthesky. All rights reserved.
// (https://github.com/Andreyinthesky/cpp-cryptography/blob/master/algorithms/gost_stribog.h).
//

#ifndef STRIBOG_H
#define STRIBOG_H

#include <vector>
#include <cstdint>

enum HashLength {
    b256 = 0,
    b512 = 1
};

class Stribog {
public:
    Stribog(HashLength hash_len = b256);
    std::vector<uint8_t> calculate(const std::vector<uint8_t>& msg);
    static std::vector<uint8_t> hash256(const std::vector<uint8_t>& msg);
    static std::vector<uint8_t> hash512(const std::vector<uint8_t>& msg);

private:
    static const uint8_t Pi[256];
    static const int Tau[64];
    static const uint64_t IV_256[8];
    static const uint64_t IV_512[8];
    static const uint64_t C[12][8];   // константы для раундов

    HashLength hash_len;
    std::vector<uint8_t> message;

    void padding();
    void processBlock(uint64_t* h, uint64_t* N, uint64_t* Sigma, const uint8_t* block);
    void lps(uint8_t* block);
    void g(uint64_t* h, const uint64_t* N, const uint64_t* m);
    void addMod512(uint64_t* a, const uint64_t* b);
    void xor512(uint64_t* result, const uint64_t* a, const uint64_t* b);
    void e(uint64_t* K, const uint64_t* m);
};


#endif //STRIBOG_H
