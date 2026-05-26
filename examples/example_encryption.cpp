#include <CryptoComponent.h>
#include "console_utf8.h"
#include <iostream>
#include <iomanip>
#include <vector>

int main() {
    setupConsoleUtf8();

    // RC4
    std::string rc4_key = "secret";
    std::string rc4_plain = "Hello, RC4!";
    std::string rc4_encrypted = cryptocomponent::CryptoComponent::rc4String(rc4_key, rc4_plain);
    std::cout << "RC4:\n  исходный текст: " << rc4_plain
              << "\n  зашифровано (hex): " << rc4_encrypted << "\n\n";

    // DES
    uint64_t des_plain = 0x3132333435363738; // "12345678" в ASCII
    uint64_t des_key   = 0x133457799BBCDFF1;
    uint64_t des_encrypted = cryptocomponent::CryptoComponent::desEncryptBlock(des_plain, des_key);
    std::cout << "DES (один блок):\n  исходный блок: 0x" << std::hex << des_plain
              << "\n  зашифрованный: 0x" << des_encrypted << std::dec << "\n\n";

    // Atbash
    std::string atbash_plain = "Hello World";
    std::string atbash_encrypted = cryptocomponent::CryptoComponent::atbash(atbash_plain);
    std::cout << "Atbash:\n  исходный текст: " << atbash_plain
              << "\n  зашифровано: " << atbash_encrypted << "\n\n";

    // Шифр Цезаря
    std::string caesar_plain = "Attack at dawn";
    int shift = 3;
    std::string caesar_encrypted = cryptocomponent::CryptoComponent::caesar(caesar_plain, shift);
    std::cout << "Цезарь (сдвиг " << shift << "):\n  исходный текст: " << caesar_plain
              << "\n  зашифровано: " << caesar_encrypted << "\n\n";

    // XOR
    std::string xor_plain = "Confidential";
    std::string xor_key   = "key123";
    std::string xor_encrypted = cryptocomponent::CryptoComponent::xorCipherString(xor_plain, xor_key);
    std::cout << "XOR:\n  исходный текст: " << xor_plain
              << "\n  ключ: " << xor_key
              << "\n  результат (hex): " << xor_encrypted << std::endl;

    return 0;
}
