#include <CryptoComponent.h>
#include <iostream>
#include <iomanip>

int main() {
    std::string text = "Hello, cryptography!";

    // Строка
    std::cout << "Хеширование строки: \"" << text << "\"\n";
    std::cout << "MD5:     " << cryptocomponent::CryptoComponent::md5(text) << "\n";
    std::cout << "SHA-256: " << cryptocomponent::CryptoComponent::sha256(text) << "\n";
    std::cout << "Стрибог-256: " << cryptocomponent::CryptoComponent::stribog256Hex(text) << "\n";

    // Файл
    std::string path = "test.txt";
    if (FILE* f = fopen(path.c_str(), "r")) {
        fclose(f);
        std::cout << "MD5 файла " << path << ": " << cryptocomponent::CryptoComponent::md5File(path) << "\n";
    } else {
        std::cout << "Файл " << path << " не найден!!!\n";
    }

    return 0;
}