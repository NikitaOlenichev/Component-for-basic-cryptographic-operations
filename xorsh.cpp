#include <iostream>
#include <string>
#include <fstream>
#include <limits>
const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

//шифрование base64
std::string base64_encode(const std::string& data) {
    std::string encoded;
    int i = 0, j = 0;
    unsigned char char_array_3[3], char_array_4[4];

    for (auto& c : data) {
        char_array_3[i++] = c;
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; i < 4; i++)
                encoded += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

        for (j = 0; j < i + 1; j++)
            encoded += base64_chars[char_array_4[j]];

        while (i++ < 3)
            encoded += '=';
    }

    return encoded;
}

//дешифрование base64
std::string base64_decode(const std::string& encoded) {
    std::string decoded;
    int i = 0, j = 0, in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];

    for (auto& c : encoded) {
        if (c == '=') break;
        char_array_4[i++] = c;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; i < 3; i++)
                decoded += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = 0; j < i; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

        for (j = 0; j < i - 1; j++)
            decoded += char_array_3[j];
    }

    return decoded;
}
// XOR-шифрование с многоразовым ключом
std::string xorCrypt(const std::string& data, const std::string& key) {
    std::string result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ key[i % key.size()];  // Циклически применяем ключ
    }
    return result;
}
// Шифрование файла
void encryptFile(const std::string& filename, const std::string& key) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        std::cerr << "File opening error!" << std::endl;
        return;
    }

    std::string content((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    std::string encrypted = xorCrypt(content, key);
    std::string encoded = base64_encode(encrypted);

    std::ofstream outFile(filename + ".enc");
    outFile << encoded;
    std::cout << "The file is encrypted and saved as " << filename + ".enc" << std::endl;
}

// Дешифрование файла
void decryptFile(const std::string& filename, const std::string& key) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "File opening error!" << std::endl;
        return;
    }

    std::string encoded((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    std::string encrypted = base64_decode(encoded);
    std::string decrypted = xorCrypt(encrypted, key);

    std::string outFilename = filename.substr(0, filename.find_last_of('.'));
    std::ofstream outFile(outFilename);
    outFile << decrypted;
    std::cout << "The file is decrypted and saved as " << outFilename << std::endl;
}
int main() {
    int choice;
    std::cout << "1.Encrypt the string\n2.Decrypt the string\n3.Encrypt the file\n4.Decrypt the file\nSelect an action: ";
    std::cin >> choice;
    std::cin.ignore();  // Очистка буфера

    if (choice == 1 || choice == 2) {
        std::string text, key;
        std::cout << "Enter the text: ";
        std::getline(std::cin, text);
        std::cout << "Enter the key: ";
        std::getline(std::cin, key);

        if (choice == 1) {
            std::string encrypted = xorCrypt(text, key);
            std::string encoded = base64_encode(encrypted);
            std::cout << "Encrypted text (Base64): " << encoded << std::endl;
        }
        else {
            std::string decoded = base64_decode(text);
            std::string decrypted = xorCrypt(decoded, key);
            std::cout << "The decrypted text: " << decrypted << std::endl;
        }
    }
    else if (choice == 3 || choice == 4) {
        std::string filename, key;
        std::cout << "Enter the file name: ";
        std::getline(std::cin, filename);
        std::cout << "Enter the key: ";
        std::getline(std::cin, key);

        if (choice == 3) {
            encryptFile(filename, key);
        }
        else {
            decryptFile(filename, key);
        }
    }
    else {
        std::cout << "Wrong choice!" << std::endl;
    }

    return 0;
}

