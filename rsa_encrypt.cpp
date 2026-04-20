#include "rsa_encrypt.h"
#include <sstream>
#include <iomanip>

using namespace std;
using namespace RSAEncrypt;

// Ѕыстрое возведение в степень по модулю (a^b % m)
long long RSAEncrypt::modPow(long long a, long long b, long long m) {
    long long result = 1;
    a = a % m;
    while (b > 0) {
        if (b & 1) result = (result * a) % m;
        a = (a * a) % m;
        b >>= 1;
    }
    return result;
}

// Ўифрование одного числа
long long RSAEncrypt::encrypt(long long message, long long e, long long n) {
    return modPow(message, e, n);
}

// Ўифрование строки (каждый символ отдельно)
string RSAEncrypt::encryptString(const string& text, long long e, long long n) {
    stringstream result;
    for (char c : text) {
        long long encrypted = encrypt((long long)c, e, n);
        result << encrypted << " ";
    }
    return result.str();
}

// Ўифрование строки с выводом в hex (шестнадцатеричный формат)
string RSAEncrypt::encryptStringHex(const string& text, long long e, long long n) {
    stringstream result;
    for (char c : text) {
        long long encrypted = encrypt((long long)c, e, n);
        result << "0x" << hex << encrypted << " ";
    }
    return result.str();
}