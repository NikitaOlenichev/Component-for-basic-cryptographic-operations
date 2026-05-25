#ifndef DESCYPHER_H
#define DESCYPHER_H

#include <cstdint>
#include <string>
#include <vector>

class DEScypher {
public:
    // Шифрование одного 64-битного блока
    static uint64_t encrypt(uint64_t block, uint64_t key);
    
    // Шифрование строки (автоматически дополняет до 8 байт нулями)
    static std::string encryptString(const std::string& plaintext, uint64_t key);
    
    // Вспомогательные функции для работы со строками
    static uint64_t stringToBlock(const std::string& str);
    static std::string blockToString(uint64_t block);

private:
    // Таблицы перестановок
    static const int IP[64];        // Initial Permutation
    static const int E[48];         // Expansion table
    static const int S[8][4][16];   // S-boxes
    static const int P[32];         // Permutation P
    static const int FP[64];        // Final Permutation (IP⁻¹)
    static const int PC1[56];       // Key schedule: PC-1
    static const int PC2[48];       // Key schedule: PC-2
    static const int shiftBits[16]; // Shift amounts for key schedule
    
    // Внутренние функции
    static uint64_t permute(uint64_t input, const int* table, int n);
    static uint32_t leftRotate(uint32_t value, int shift);
    static std::vector<uint64_t> generateKeys(uint64_t key);
    static uint64_t desProcess(uint64_t block, const std::vector<uint64_t>& roundKeys);
};

#endif // DESCYPHER_H
