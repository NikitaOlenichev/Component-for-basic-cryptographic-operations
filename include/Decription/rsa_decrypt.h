#ifndef RSA_DECRYPT_H
#define RSA_DECRYPT_H

#include <string>
#include <vector>

namespace RSADecrypt {
    // Быстрое возведение в степень по модулю
    long long modPow(long long a, long long b, long long m);

    // Расшифрование одного числа
    long long decrypt(long long ciphertext, long long d, long long n);

    // Расшифрование строки (формат: "число число число ...")
    std::string decryptString(const std::string& ciphertext, long long d, long long n);

    // Расшифрование из HEX формата (формат: "0xAAA 0xBBB ...")
    std::string decryptStringHex(const std::string& hexCiphertext, long long d, long long n);

    // Расшифрование из вектора чисел
    std::string decryptVector(const std::vector<long long>& ciphertext, long long d, long long n);
}

#endif // RSA_DECRYPT_H