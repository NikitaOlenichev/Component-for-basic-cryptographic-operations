#include "Decryption/rsa_decrypt.h"
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace RSADecrypt;

// ������� ���������� � ������� �� ������ (a^b % m)
long long RSADecrypt::modPow(long long a, long long b, long long m) {
    long long result = 1;
    a = a % m;
    while (b > 0) {
        if (b & 1) result = (result * a) % m;
        a = (a * a) % m;
        b >>= 1;
    }
    return result;
}

// ������������� ������ �����
long long RSADecrypt::decrypt(long long ciphertext, long long d, long long n) {
    return modPow(ciphertext, d, n);
}

// ������������� ������ (������: "����� ����� ����� ...")
string RSADecrypt::decryptString(const string& ciphertext, long long d, long long n) {
    stringstream ss(ciphertext);
    string result;
    long long number;

    while (ss >> number) {
        long long decrypted = decrypt(number, d, n);
        // ���������, ��� �������������� �������� - ��� ���������� ������
        if (decrypted >= 0 && decrypted <= 255) {
            result += static_cast<char>(decrypted);
        }
        else {
            result += '?'; // ���� �������� ������� �� ������� ASCII
        }
    }
    return result;
}

// ������������� �� HEX ������� (������: "0xAAA 0xBBB ...")
string RSADecrypt::decryptStringHex(const string& hexCiphertext, long long d, long long n) {
    stringstream ss(hexCiphertext);
    string result;
    string hexNumber;

    while (ss >> hexNumber) {
        // ������� ������� "0x" ���� �� ����
        if (hexNumber.substr(0, 2) == "0x" || hexNumber.substr(0, 2) == "0X") {
            hexNumber = hexNumber.substr(2);
        }

        long long number;
        stringstream hexStream;
        hexStream << hex << hexNumber;
        hexStream >> number;

        long long decrypted = decrypt(number, d, n);
        if (decrypted >= 0 && decrypted <= 255) {
            result += static_cast<char>(decrypted);
        }
        else {
            result += '?';
        }
    }
    return result;
}

// ������������� �� ������� �����
string RSADecrypt::decryptVector(const vector<long long>& ciphertext, long long d, long long n) {
    string result;
    for (long long number : ciphertext) {
        long long decrypted = decrypt(number, d, n);
        if (decrypted >= 0 && decrypted <= 255) {
            result += static_cast<char>(decrypted);
        }
        else {
            result += '?';
        }
    }
    return result;
}