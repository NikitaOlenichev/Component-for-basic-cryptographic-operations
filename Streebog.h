//
// Реализует хеш-функцию по ГОСТ Р 34.11-2012 (Стрибог),
// которая преобразует сообщение произвольной длины в хеш-код фиксированного размера (256 или 512 бит).
//

#ifndef STREEBOG_H
#define STREEBOG_H

#include <vector>
#include <cstdint>

class Streebog {
public:
    Streebog(int outputSize = 512);

    void update(const uint8_t* data, size_t length);
    void finalize(uint8_t* hash);

    static std::vector<uint8_t> hash(const uint8_t* data, size_t length, int outputSize = 512);
    static std::vector<uint8_t> hash(const std::vector<uint8_t>& data, int outputSize = 512);

private:
    static const size_t BLOCK_SIZE = 64;
    static const uint64_t IV_256[8];
    static const uint64_t IV_512[8];

    int outputSize;
    uint8_t buffer[BLOCK_SIZE];
    size_t bufferLength;
    uint64_t totalBits;
    uint64_t h[8];
    uint64_t N[8];
    uint64_t Sigma[8];

    void processBlock(const uint8_t* block);
    void padAndFinalize();
    void g(uint64_t* h, const uint64_t* m);
    void addMod512(uint64_t* a, const uint64_t* b);
    void xorBlocks(uint64_t* result, const uint64_t* a, const uint64_t* b);
    void lpsTransformation(uint8_t* block);
    void keySchedule(uint64_t* k, const uint64_t* h, const uint64_t* m);
    void encrypt(uint64_t* state, const uint64_t* key);
};

#endif //STREEBOG_H
