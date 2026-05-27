#include <CryptoComponent.h>
#include "console_utf8.h"
#include <iostream>
#include <iomanip>

int main() {
    setupConsoleUtf8();

    std::string text = "Hello, cryptography!";
    std::string key = "supersecret";

    // Строка
#ifdef USE_OPENSSL
    std::cout << "Хеширование строки: " << text << "\n";
    std::cout << "MD5: " << cryptocomponent::CryptoComponent::md5(text) << "\n";
    std::cout << "SHA-1: " << cryptocomponent::CryptoComponent::sha1(text) << "\n";
    std::cout << "SHA-256: " << cryptocomponent::CryptoComponent::sha256(text) << "\n";
    std::cout << "SHA-512: " << cryptocomponent::CryptoComponent::sha512(text) << "\n";
    std::cout << "HMAC-SHA256 (key=" << key << "): " << cryptocomponent::CryptoComponent::hmacSHA256(text, key) << "\n";

    // Файл
    std::string path = "test.txt";
    if (FILE* f = fopen(path.c_str(), "r")) {
        fclose(f);
        std::cout << "MD5 файла " << path << ": " << cryptocomponent::CryptoComponent::md5File(path) << "\n";
    } else {
        std::cout << "Файл " << path << " не найден!!!\n";
    }

    // Универсальный метод
    std::cout << "Универсальный метод (SHA-256): "
              << cryptocomponent::CryptoComponent::hash(text, "SHA256") << "\n";
#else
    std::cout << "OpenSSL не найден. Хеши MD5, SHA и HMAC недоступны.\n";
#endif

    // Гост Stribog
    std::cout << "Stribog-256: " << cryptocomponent::CryptoComponent::stribog256Hex(text) << "\n";
    std::cout << "Stribog-512: " << cryptocomponent::CryptoComponent::stribog512Hex(text) << "\n";

    return 0;
}
