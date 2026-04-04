#include "rsa.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <sstream>
#include <cmath>

using namespace std;
using namespace RSA;

// Наибольший общий делитель (своя реализация для Visual Studio)
long long RSA::gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Быстрое возведение в степень по модулю (a^b % m)
long long RSA::modPow(long long a, long long b, long long m) {
    long long result = 1;
    a = a % m;
    while (b > 0) {
        if (b & 1) result = (result * a) % m;
        a = (a * a) % m;
        b >>= 1;
    }
    return result;
}

// Расширенный алгоритм Евклида для нахождения обратного числа
long long RSA::modInverse(long long a, long long m) {
    long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;

    if (m == 1) return 0;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0) x1 += m0;
    return x1;
}

// Простая проверка простоты (для маленьких чисел)
bool RSA::isPrime(long long n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// Генерация простого числа в заданном диапазоне
long long RSA::generatePrime(long long min, long long max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<long long> dist(min, max);

    long long prime;
    do {
        prime = dist(gen);
    } while (!isPrime(prime));

    return prime;
}

// Генерация ключей RSA (адаптировано для Visual Studio)
void RSA::generateKeys(long long& n, long long& e, long long& d) {
    // Для учебных целей используем фиксированные числа
    // В реальном проекте раскомментируйте строки ниже для случайных
    long long p = 61;
    long long q = 53;

    // long long p = generatePrime(50, 100);
    // long long q = generatePrime(50, 100);
    // while (p == q) q = generatePrime(50, 100);

    n = p * q;
    long long phi = (p - 1) * (q - 1);

    e = 17; // Стандартное значение
    // ИСПРАВЛЕНО: используем свою функцию gcd вместо __gcd
    while (gcd(e, phi) != 1) {
        e += 2;
    }

    d = modInverse(e, phi);
}

// Шифрование
long long RSA::encrypt(long long message, long long e, long long n) {
    return modPow(message, e, n);
}

// Расшифрование
long long RSA::decrypt(long long ciphertext, long long d, long long n) {
    return modPow(ciphertext, d, n);
}

// Шифрование строки (каждый символ отдельно)
string RSA::encryptString(const string& text, long long e, long long n) {
    stringstream result;
    for (char c : text) {
        long long encrypted = encrypt((long long)c, e, n);
        result << encrypted << " ";
    }
    return result.str();
}

// Расшифрование строки
string RSA::decryptString(const string& ciphertext, long long d, long long n) {
    stringstream ss(ciphertext);
    string result;
    long long number;

    while (ss >> number) {
        char decrypted = static_cast<char>(decrypt(number, d, n));
        result += decrypted;
    }
    return result;
}