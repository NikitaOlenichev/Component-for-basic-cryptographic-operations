#include "HashCalculate.h"

#include <openssl/evp.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdexcept>

using namespace std;

const EVP_MD* getMD(const string& algorithm) {
    if (algorithm == "MD5") return EVP_md5();
    if (algorithm == "SHA1") return EVP_sha1();
    if (algorithm == "SHA256") return EVP_sha256();
    if (algorithm == "SHA512") return EVP_sha512();
    return nullptr;
}

string HashCalculate::hashString(const string& input, const string& algorithm) {
    const EVP_MD* md = getMD(algorithm);
    if (!md) {
        throw runtime_error("Неподдерживаемый алгоритм: " + algorithm);
    }

    EVP_MD_CTX* context = EVP_MD_CTX_new();
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int length = 0;

    EVP_DigestInit_ex(context, md, nullptr);
    EVP_DigestUpdate(context, input.c_str(), input.size());
    EVP_DigestFinal_ex(context, hash, &length);

    EVP_MD_CTX_free(context);
    return toHex(hash, length);
}

string HashCalculate::hashFile(const string& filePath, const string& algorithm) {
    const EVP_MD* md = getMD(algorithm);
    if (!md) {
        throw runtime_error("Неподдерживаемый алгоритм: " + algorithm);
    }

    ifstream file(filePath, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл");
    }

    EVP_MD_CTX* context = EVP_MD_CTX_new();
    EVP_DigestInit_ex(context, md, nullptr);

    vector<char> buffer(4096);
    while (file.good()) {
        file.read(buffer.data(), buffer.size());
        streamsize bytes = file.gcount();
        if (bytes > 0) {
            EVP_DigestUpdate(context, buffer.data(), bytes);
        }
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int length = 0;

    EVP_DigestFinal_ex(context, hash, &length);
    EVP_MD_CTX_free(context);

    return toHex(hash, length);
}

string HashCalculate::toHex(const unsigned char* hash, unsigned int length) {
    stringstream ss;
    for (unsigned int i = 0; i < length; i++) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}