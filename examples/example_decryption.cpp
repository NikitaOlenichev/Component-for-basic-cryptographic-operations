#include <CryptoComponent.h>
#include <iostream>
#include <iomanip>
#include <vector>

int main() {
    // RC4
    std::string rc4_key = "secret";
    std::string rc4_plain = "Hello, RC4!";
    std::string rc4_encrypted = cryptocomponent::CryptoComponent::rc4String(rc4_key, rc4_plain);
    std::string rc4_decrypted = cryptocomponent::CryptoComponent::rc4String(rc4_key, rc4_encrypted);
    std::cout << "RC4:\n  зашифровано (hex): " << rc4_encrypted
              << "\n  расшифровано: " << rc4_decrypted << "\n\n";

    // DES
    uint64_t des_cipher = 0xE0C3B2A1D5F4E6C7;
    uint64_t des_key = 0x133457799BBCDFF1;
    uint64_t des_decrypted = cryptocomponent::CryptoComponent::desDecryptBlock(des_cipher, des_key);
    std::cout << "DES:\n  зашифрованный блок: 0x" << std::hex << des_cipher
              << "\n  расшифрованный блок: 0x" << des_decrypted
              << " -> текст: \"12345678\" (если ключ и блок верны)" << std::dec << "\n\n";

    // Atbash
    std::string atbash_cipher = cryptocomponent::CryptoComponent::atbash("Svool Dliow");
    std::cout << "Atbash:\n  зашифрованный текст: Svool Dliow"
              << "\n  расшифровано: " << atbash_cipher << "\n\n";

    // Шифр Цезаря
    std::string caesar_cipher = "Dwwdfn dw gdzq";
    int shift_encrypt = 3;
    int shift_decrypt = -shift_encrypt;
    std::string caesar_decrypted = cryptocomponent::CryptoComponent::caesar(caesar_cipher, shift_decrypt);
    std::cout << "Цезарь:\n  зашифрованный текст: " << caesar_cipher
              << "\n  расшифровано (сдвиг " << shift_encrypt << "): " << caesar_decrypted << "\n\n";

    // XOR
    std::string xor_key = "key123";
    std::string xor_encrypted_hex = "1c0d1e1f080b0a09";
    std::string xor_plain = "Confidential";
    std::string xor_encrypted = cryptocomponent::CryptoComponent::xorCipherString(xor_plain, xor_key);
    std::string xor_decrypted = cryptocomponent::CryptoComponent::xorCipherString(xor_encrypted, xor_key);
    std::cout << "XOR:\n  зашифровано (hex): " << xor_encrypted
              << "\n  расшифровано: " << xor_decrypted << std::endl;

    return 0;
}
