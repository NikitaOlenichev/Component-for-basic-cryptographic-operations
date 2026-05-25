#include "Encryption/rc4_encrypt.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;


RC4_Encryption::RC4_Encryption() {
    i = 0;
    j = 0;
    fill(begin(S), end(S), 0);
}

RC4_Encryption::RC4_Encryption(const vector<unsigned char>& key) {
    setKey(key);
}

RC4_Encryption::RC4_Encryption(const string& key) {
    setKey(key);
}

void RC4_Encryption::initSBox(const vector<unsigned char>& key) {
    
    for (int k = 0; k < 256; k++) {
        S[k] = k;
    }

    
    int keyLen = key.size();
    int j = 0;

    for (int i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % keyLen]) % 256;
        swap(S[i], S[j]);
    }

    
    this->i = 0;
    this->j = 0;
}

void RC4_Encryption::setKey(const vector<unsigned char>& key) {
    if (key.empty()) {
        
        vector<unsigned char> defaultKey = { 0, 1, 2, 3, 4, 5, 6, 7 };
        initSBox(defaultKey);
    }
    else {
        initSBox(key);
    }
}

void RC4_Encryption::setKey(const string& key) {
    vector<unsigned char> keyBytes(key.begin(), key.end());
    setKey(keyBytes);
}

unsigned char RC4_Encryption::generateKeystreamByte() {
    // Генерация следующего байта keystream
    i = (i + 1) % 256;
    j = (j + S[i]) % 256;
    swap(S[i], S[j]);

    int k = (S[i] + S[j]) % 256;
    return S[k];
}

vector<unsigned char> RC4_Encryption::encrypt(const vector<unsigned char>& data) {
    vector<unsigned char> result;
    result.reserve(data.size());

    // Сохраняем текущее состояние
    unsigned char savedS[256];
    int savedI = i;
    int savedJ = j;
    copy(begin(S), end(S), begin(savedS));

    // Шифрование: data XOR keystream
    for (size_t k = 0; k < data.size(); k++) {
        unsigned char keystreamByte = generateKeystreamByte();
        result.push_back(data[k] ^ keystreamByte);
    }

    // Восстанавливаем состояние
    copy(begin(savedS), end(savedS), begin(S));
    i = savedI;
    j = savedJ;

    return result;
}

vector<unsigned char> RC4_Encryption::encrypt(const string& data) {
    vector<unsigned char> dataBytes(data.begin(), data.end());
    return encrypt(dataBytes);
}

void RC4_Encryption::reset() {
    i = 0;
    j = 0;
}


void printHex(const vector<unsigned char>& data) {
    for (unsigned char byte : data) {
        cout << hex << setw(2) << setfill('0') << (int)byte;
    }
    cout << dec << endl;
}

void printBytes(const vector<unsigned char>& data) {
    for (unsigned char byte : data) {
        cout << (int)byte << " ";
    }
    cout << endl;
}