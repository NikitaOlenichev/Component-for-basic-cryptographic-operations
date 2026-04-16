#ifndef DES_H
#define DES_H

#include <cstdint>
#include <string>
#include <vector>

class DES {
public:
    // Шифрование одного 64-битного блока
    static uint64_t encrypt(uint64_t block, uint64_t key);
    
    // Дешифрование одного 64-битного блока
    static uint64_t decrypt(uint64_t block, uint64_t key);
    
    // Вспомогательные функции для работы со строками
    static uint64_t stringToBlock(const std::string& str);
    static std::string blockToString(uint64_t block);
    
    // Шифрование строки (автоматически разбивает на блоки по 8 байт)
    static std::string encryptString(const std::string& plaintext, uint64_t key);
    
    // Дешифрование строки (автоматически разбивает на блоки по 8 байт)
    static std::string decryptString(const std::string& ciphertext, uint64_t key);

private:
    // Таблицы перестановок
    static const int IP[64];
    static const int E[48];
    static const int S[8][4][16];
    static const int P[32];
    static const int FP[64];
    static const int PC1[56];
    static const int PC2[48];
    static const int shiftBits[16];
    
    // Внутренние функции
    static uint64_t permute(uint64_t input, const int* table, int n);
    static uint32_t leftRotate(uint32_t value, int shift);
    static std::vector<uint64_t> generateKeys(uint64_t key);
    static uint64_t desProcess(uint64_t block, const std::vector<uint64_t>& roundKeys);
};

#endif // DES_H
