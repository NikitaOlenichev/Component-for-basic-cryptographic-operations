#ifndef RC4_ENCRYPT_H
#define RC4_ENCRYPT_H

#include <vector>
#include <string>

class RC4_Encryption {
private:
    unsigned char S[256];  // S-box
    int i, j;              // состояние для генерации keystream

    void initSBox(const std::vector<unsigned char>& key);
    unsigned char generateKeystreamByte();

public:
    RC4_Encryption();
    RC4_Encryption(const std::vector<unsigned char>& key);
    RC4_Encryption(const std::string& key);

    void setKey(const std::vector<unsigned char>& key);
    void setKey(const std::string& key);

    // Только шифрование
    std::vector<unsigned char> encrypt(const std::vector<unsigned char>& data);
    std::vector<unsigned char> encrypt(const std::string& data);

    void reset();
};

#endif // RC4_ENCRYPT_H
