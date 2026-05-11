#ifndef HASHCALCULATE_H
#define HASHCALCULATE_H

#include <string>

class HashCalculate {
public:
    // Хеш строки
    static std::string hashString(const std::string& input, const std::string& algorithm = "SHA256");

    // Хеш файла
    static std::string hashFile(const std::string& filePath, const std::string& algorithm = "SHA256");

private:
    static const void* getAlgorithm(const std::string& algorithm);
    static std::string toHex(const unsigned char* hash, unsigned int length);
};

#endif