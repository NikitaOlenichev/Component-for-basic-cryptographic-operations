//
// Реализует хеш-функцию по ГОСТ Р 34.11-2012 (Стрибог),
// которая преобразует сообщение произвольной длины в хеш-код фиксированного размера (256 или 512 бит).
//

#include "Streebog.h"
#include <cstring>
#include <algorithm>

const uint64_t Streebog::IV_256[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // заполнить реальными значения
const uint64_t Streebog::IV_512[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // заполнить реальными значения

Streebog::Streebog(int outputSize) : outputSize(outputSize), bufferLength(0), totalBits(0) {
    if (outputSize != 256 && outputSize != 512) {
        throw std::runtime_error("Invalid output size. Must be 256 or 512.");
    }

    const uint64_t* iv = (outputSize == 256) ? IV_256 : IV_512;
    memcpy(h, iv, 8 * sizeof(uint64_t));
    memset(N, 0, 8 * sizeof(uint64_t));
    memset(Sigma, 0, 8 * sizeof(uint64_t));
    memset(buffer, 0, BLOCK_SIZE);
}

void Streebog::update(const uint8_t* data, size_t length) {
    totalBits += length * 8;

    while (length > 0) {
        size_t toCopy = std::min(BLOCK_SIZE - bufferLength, length);
        memcpy(buffer + bufferLength, data, toCopy);
        bufferLength += toCopy;
        length -= toCopy;
        data += toCopy;

        if (bufferLength == BLOCK_SIZE) {
            processBlock(buffer);
            bufferLength = 0;
        }
    }
}

void Streebog::finalize(uint8_t* hash) {
    padAndFinalize();

    if (outputSize == 256) {
        memcpy(hash, h, 32);
    } else {
        memcpy(hash, h, 64);
    }
}

void Streebog::processBlock(const uint8_t* block) {
    uint64_t m[8];
    for (int i = 0; i < 8; ++i) {
        m[i] = 0;
        for (int j = 0; j < 8; ++j) {
            m[i] |= (static_cast<uint64_t>(block[i * 8 + j]) << (j * 8));
        }
    }

    uint64_t h_new[8];
    memcpy(h_new, h, 8 * sizeof(uint64_t));

    uint64_t k[8];
    keySchedule(k, h_new, m);

    encrypt(h_new, k);

    addMod512(N, m);
    addMod512(Sigma, h_new);

    memcpy(h, h_new, 8 * sizeof(uint64_t));
}

void Streebog::padAndFinalize() {
    uint8_t padding[BLOCK_SIZE];
    memset(padding, 0, BLOCK_SIZE);

    padding[0] = 0x01;
    size_t paddingLength = BLOCK_SIZE - bufferLength;
    if (paddingLength < 1) {
        paddingLength += BLOCK_SIZE;
    }

    update(padding, paddingLength);

    uint8_t lengthBytes[16];
    for (int i = 0; i < 16; ++i) {
        lengthBytes[i] = (totalBits >> (i * 8)) & 0xFF;
    }
    update(lengthBytes, 16);

    //Финальная обработка
}

void Streebog::g(uint64_t* h, const uint64_t* m) {
    // реализовать функцию g
}

void Streebog::addMod512(uint64_t* a, const uint64_t* b) {
    uint64_t carry = 0;
    for (int i = 0; i < 8; ++i) {
        uint64_t sum = a[i] + b[i] + carry;
        a[i] = sum;
        carry = (sum < a[i] || sum < b[i]) ? 1 : 0;
    }
}

void Streebog::xorBlocks(uint64_t* result, const uint64_t* a, const uint64_t* b) {
    for (int i = 0; i < 8; ++i) {
        result[i] = a[i] ^ b[i];
    }
}

void Streebog::lpsTransformation(uint8_t* block) {
    // реализовать LPS преобразование
}

void Streebog::keySchedule(uint64_t* k, const uint64_t* h, const uint64_t* m) {
    // реализовать ключевое расписание
}

void Streebog::encrypt(uint64_t* state, const uint64_t* key) {
    // реализовать шифрование
}

std::vector<uint8_t> Streebog::hash(const uint8_t* data, size_t length, int outputSize) {
    Streebog streebog(outputSize);
    streebog.update(data, length);
    std::vector<uint8_t> hash(outputSize / 8);
    streebog.finalize(hash.data());
    return hash;
}

std::vector<uint8_t> Streebog::hash(const std::vector<uint8_t>& data, int outputSize) {
    return hash(data.data(), data.size(), outputSize);
}
