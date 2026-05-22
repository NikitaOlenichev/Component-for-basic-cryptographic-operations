#ifndef HASHCALCULATE_H
#define HASHCALCULATE_H

#include <openssl/evp.h>

#include <string>
#include <vector>

class HashCalculate {
public:
    // Хеш строки
    static std::string hashString(const std::string& input, const std::string& algorithm = "SHA256");

    // Хеш файла
    static std::string hashFile(const std::string& filePath, const std::string& algorithm = "SHA256");

    // HMAC строки с секретным ключом
    static std::string hmacString(
        const std::string& input,
        const std::string& key,
        const std::string& algorithm = "SHA256"
    );

    // Список поддерживаемых алгоритмов
    static std::vector<std::string> supportedAlgorithms();

private:
    static const EVP_MD* getAlgorithm(const std::string& algorithm);
    static std::string normalizeAlgorithm(const std::string& algorithm);
    static std::string toHex(const unsigned char* hash, unsigned int length);
};

#endif
