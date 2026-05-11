#include "rc4.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

void RC4::init(const std::string& key) {
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

unsigned char RC4::generateByte() {
    i = (i + 1) % 256;
    j = (j + S[i]) % 256;
    std::swap(S[i], S[j]);
    return S[(S[i] + S[j]) % 256];
}

RC4::RC4(const std::string& key) {
    init(key);
}

std::string RC4::decryptString(const std::string& ciphertext) {
    std::string result;
    for (char c : ciphertext) {
        unsigned char keystream = generateByte();
        result += c ^ keystream;
    }
    return result;
}

void RC4::decryptFile(const std::string& inputFile, const std::string& outputFile) {
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

std::vector<unsigned char> RC4::decryptBytes(const std::vector<unsigned char>& ciphertext) {
    std::vector<unsigned char> result(ciphertext.size());
    for (size_t k = 0; k < ciphertext.size(); k++) {
        unsigned char keystream = generateByte();
        result[k] = ciphertext[k] ^ keystream;
    }
    return result;
}

int main() {
    setlocale(LC_ALL, "Russian");
    try {
        std::string key;
        int choice;

        std::cout << "=== RC4 ДЕШИФРОВАНИЕ ===" << std::endl;
        std::cout << "Введите ключ: ";
        std::getline(std::cin, key);

        std::cout << "Выберите режим:" << std::endl;
        std::cout << "1. Дешифровать строку" << std::endl;
        std::cout << "2. Дешифровать файл" << std::endl;
        std::cout << "3. Дешифровать hex строку" << std::endl;
        std::cout << "Ваш выбор: ";
        std::cin >> choice;
        std::cin.ignore();

        RC4 rc4(key);

        switch (choice) {
        case 1: {
            std::string ciphertext;
            std::cout << "Введите зашифрованную строку: ";
            std::getline(std::cin, ciphertext);

            std::string decrypted = rc4.decryptString(ciphertext);
            std::cout << "\nРасшифрованный текст: " << decrypted << std::endl;
            break;
        }
        case 2: {
            std::string inputFile, outputFile;
            std::cout << "Введите имя зашифрованного файла: ";
            std::getline(std::cin, inputFile);
            std::cout << "Введите имя для расшифрованного файла: ";
            std::getline(std::cin, outputFile);

            rc4.decryptFile(inputFile, outputFile);
            std::cout << "\nФайл успешно расшифрован: " << outputFile << std::endl;
            break;
        }
        case 3: {
            std::string hexString;
            std::cout << "Введите hex строку: ";
            std::getline(std::cin, hexString);

            std::vector<unsigned char> ciphertext;
            for (size_t i = 0; i < hexString.length(); i += 2) {
                std::string byteString = hexString.substr(i, 2);
                unsigned char byte = (unsigned char)strtol(byteString.c_str(), nullptr, 16);
                ciphertext.push_back(byte);
            }

            std::vector<unsigned char> decrypted = rc4.decryptBytes(ciphertext);
            std::string result(decrypted.begin(), decrypted.end());
            std::cout << "\nРасшифрованный текст: " << result << std::endl;
            break;
        }
        default:
            std::cout << "Неверный выбор!" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
