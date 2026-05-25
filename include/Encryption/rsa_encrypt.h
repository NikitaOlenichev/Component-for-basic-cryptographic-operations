#ifndef RSA_ENCRYPT_H
#define RSA_ENCRYPT_H

#include <string>

namespace RSAEncrypt {
    // Ѕыстрое возведение в степень по модулю
    long long modPow(long long a, long long b, long long m);

    // Ўифрование одного числа
    long long encrypt(long long message, long long e, long long n);

    // Ўифрование строки (каждый символ отдельно)
    std::string encryptString(const std::string& text, long long e, long long n);

    // Ўифрование строки с выводом в hex (дл€ удобного отображени€)
    std::string encryptStringHex(const std::string& text, long long e, long long n);
}

#endif // RSA_ENCRYPT_H