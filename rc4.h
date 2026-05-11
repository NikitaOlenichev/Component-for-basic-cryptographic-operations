#ifndef RC4_H
#define RC4_H

#include <string>
#include <vector>

class RC4 {
private:
    unsigned char S[256];
    int i, j;

    void init(const std::string& key);
    unsigned char generateByte();

public:
    RC4(const std::string& key);

    // Дешифрование строки
    std::string decryptString(const std::string& ciphertext);

    // Дешифрование файла
    void decryptFile(const std::string& inputFile, const std::string& outputFile);

    // Дешифрование вектора байт
    std::vector<unsigned char> decryptBytes(const std::vector<unsigned char>& ciphertext);
};

#endif // RC4_H
