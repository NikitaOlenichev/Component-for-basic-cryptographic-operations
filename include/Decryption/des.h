#ifndef DES_H
#define DES_H

#include <cstdint>
#include <string>
#include <vector>

class DES {
public:
    // Дешифрование одного 64-битного блока
    static uint64_t decrypt(uint64_t block, uint64_t key);
    
    // Дешифрование строки (автоматически разбивает на блоки по 8 байт)
    static std::string decryptString(const std::string& ciphertext, uint64_t key);
    
    // Вспомогательные функции для работы со строками
    static uint64_t stringToBlock(const std::string& str);
    static std::string blockToString(uint64_t block);
    
    // Преобразование строки hex в число
    static uint64_t hexToBlock(const std::string& hex);
};

#endif // DES_H
