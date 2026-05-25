#include "Decryption/rc4_decrypt.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

void RC4_Decryption::init(const std::string& key) {
    for (int k = 0; k < 256; k++) {
        S[k] = k;
    }

    j = 0;
    for (int k = 0; k < 256; k++) {
        j = (j + S[k] + key[k % key.length()]) % 256;
        std::swap(S[k], S[j]);
    }

    i = 0;
    j = 0;
}

unsigned char RC4_Decryption::generateByte() {
    i = (i + 1) % 256;
    j = (j + S[i]) % 256;
    std::swap(S[i], S[j]);
    return S[(S[i] + S[j]) % 256];
}

RC4_Decryption::RC4_Decryption(const std::string& key) {
    init(key);
}

std::string RC4_Decryption::decryptString(const std::string& ciphertext) {
    std::string result;
    for (char c : ciphertext) {
        unsigned char keystream = generateByte();
        result += c ^ keystream;
    }
    return result;
}

void RC4_Decryption::decryptFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);

    if (!inFile.is_open()) {
        throw std::runtime_error("Не удалось открыть входной файл: " + inputFile);
    }

    if (!outFile.is_open()) {
        throw std::runtime_error("Не удалось открыть выходной файл: " + outputFile);
    }

    char buffer[4096];
    while (inFile.read(buffer, sizeof(buffer)) || inFile.gcount() > 0) {
        for (std::streamsize k = 0; k < inFile.gcount(); k++) {
            unsigned char keystream = generateByte();
            buffer[k] = buffer[k] ^ keystream;
        }
        outFile.write(buffer, inFile.gcount());
    }

    inFile.close();
    outFile.close();
}

std::vector<unsigned char> RC4_Decryption::decryptBytes(const std::vector<unsigned char>& ciphertext) {
    std::vector<unsigned char> result(ciphertext.size());
    for (size_t k = 0; k < ciphertext.size(); k++) {
        unsigned char keystream = generateByte();
        result[k] = ciphertext[k] ^ keystream;
    }
    return result;
}
