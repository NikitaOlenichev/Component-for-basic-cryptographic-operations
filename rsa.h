#pragma once
#ifndef RSA_H
#define RSA_H

#include <string>

namespace RSA {
    // Быстрое возведение в степень по модулю
    long long modPow(long long a, long long b, long long m);

    // Нахождение обратного числа по модулю
    long long modInverse(long long a, long long m);

    // Проверка числа на простоту
    bool isPrime(long long n);

    // Генерация простого числа в диапазоне
    long long generatePrime(long long min, long long max);

    // Наибольший общий делитель (своя реализация для VS)
    long long gcd(long long a, long long b);

    // Генерация ключей RSA
    void generateKeys(long long& n, long long& e, long long& d);

    // Шифрование одного числа
    long long encrypt(long long message, long long e, long long n);

    // Расшифрование одного числа
    long long decrypt(long long ciphertext, long long d, long long n);

    // Дополнительные утилиты для строк
    std::string encryptString(const std::string& text, long long e, long long n);
    std::string decryptString(const std::string& ciphertext, long long d, long long n);
}

#endif // RSA_H