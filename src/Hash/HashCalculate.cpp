#include "HashCalculate.h"
#include "Signatures/Stribog.h"

#include <openssl/hmac.h>

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace {
    bool isStribog256(const std::string& algorithm) {
        return algorithm == "STRIBOG256" || algorithm == "GOST34112012256";
    }

    bool isStribog512(const std::string& algorithm) {
        return algorithm == "STRIBOG512" || algorithm == "GOST34112012512";
    }

    std::vector<uint8_t> readFileBytes(const std::string& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Не удалось открыть файл: " + filePath);
        }

        return std::vector<uint8_t>(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>()
        );
    }
}

std::string HashCalculate::hashString(const std::string& input, const std::string& algorithm) {
    const std::string normalizedAlgorithm = normalizeAlgorithm(algorithm);

    if (isStribog256(normalizedAlgorithm) || isStribog512(normalizedAlgorithm)) {
        const std::vector<uint8_t> data(input.begin(), input.end());
        const std::vector<uint8_t> hash = isStribog256(normalizedAlgorithm)
            ? Stribog::hash256(data)
            : Stribog::hash512(data);
        return toHex(reinterpret_cast<const unsigned char*>(hash.data()), static_cast<unsigned int>(hash.size()));
    }

    const EVP_MD* md = getAlgorithm(normalizedAlgorithm);
    if (!md) {
        throw std::runtime_error("Неподдерживаемый алгоритм: " + algorithm);
    }

    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (!context) {
        throw std::runtime_error("Не удалось создать контекст OpenSSL EVP");
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int length = 0;

    if (EVP_DigestInit_ex(context, md, nullptr) != 1 ||
        EVP_DigestUpdate(context, input.data(), input.size()) != 1 ||
        EVP_DigestFinal_ex(context, hash, &length) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Ошибка вычисления хеша OpenSSL");
    }

    EVP_MD_CTX_free(context);
    return toHex(hash, length);
}

std::string HashCalculate::hashFile(const std::string& filePath, const std::string& algorithm) {
    const std::string normalizedAlgorithm = normalizeAlgorithm(algorithm);

    if (isStribog256(normalizedAlgorithm) || isStribog512(normalizedAlgorithm)) {
        const std::vector<uint8_t> data = readFileBytes(filePath);
        const std::vector<uint8_t> hash = isStribog256(normalizedAlgorithm)
            ? Stribog::hash256(data)
            : Stribog::hash512(data);
        return toHex(reinterpret_cast<const unsigned char*>(hash.data()), static_cast<unsigned int>(hash.size()));
    }

    const EVP_MD* md = getAlgorithm(normalizedAlgorithm);
    if (!md) {
        throw std::runtime_error("Неподдерживаемый алгоритм: " + algorithm);
    }

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + filePath);
    }

    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (!context) {
        throw std::runtime_error("Не удалось создать контекст OpenSSL EVP");
    }

    if (EVP_DigestInit_ex(context, md, nullptr) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Ошибка инициализации OpenSSL EVP");
    }

    std::vector<char> buffer(4096);
    while (file.good()) {
        file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
        const std::streamsize bytes = file.gcount();
        if (bytes > 0 && EVP_DigestUpdate(context, buffer.data(), static_cast<size_t>(bytes)) != 1) {
            EVP_MD_CTX_free(context);
            throw std::runtime_error("Ошибка чтения данных для хеша OpenSSL");
        }
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int length = 0;

    if (EVP_DigestFinal_ex(context, hash, &length) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Ошибка завершения вычисления хеша OpenSSL");
    }

    EVP_MD_CTX_free(context);
    return toHex(hash, length);
}

std::string HashCalculate::hmacString(
    const std::string& input,
    const std::string& key,
    const std::string& algorithm
) {
    const std::string normalizedAlgorithm = normalizeAlgorithm(algorithm);
    if (isStribog256(normalizedAlgorithm) || isStribog512(normalizedAlgorithm)) {
        throw std::runtime_error("HMAC для Стрибог пока не поддерживается этим модулем");
    }

    const EVP_MD* md = getAlgorithm(normalizedAlgorithm);
    if (!md) {
        throw std::runtime_error("Неподдерживаемый алгоритм HMAC: " + algorithm);
    }

    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int length = 0;

    unsigned char* result = HMAC(
        md,
        key.data(),
        static_cast<int>(key.size()),
        reinterpret_cast<const unsigned char*>(input.data()),
        input.size(),
        digest,
        &length
    );

    if (!result) {
        throw std::runtime_error("Ошибка вычисления HMAC OpenSSL");
    }

    return toHex(digest, length);
}

std::vector<std::string> HashCalculate::supportedAlgorithms() {
    return {
        "MD5",
        "SHA1",
        "SHA256",
        "SHA512",
        "STRIBOG256",
        "STRIBOG512"
    };
}

const EVP_MD* HashCalculate::getAlgorithm(const std::string& algorithm) {
    const std::string normalizedAlgorithm = normalizeAlgorithm(algorithm);

    if (normalizedAlgorithm == "MD5") return EVP_md5();
    if (normalizedAlgorithm == "SHA1") return EVP_sha1();
    if (normalizedAlgorithm == "SHA256") return EVP_sha256();
    if (normalizedAlgorithm == "SHA512") return EVP_sha512();
    return nullptr;
}

std::string HashCalculate::normalizeAlgorithm(const std::string& algorithm) {
    std::string normalized;
    normalized.reserve(algorithm.size());

    for (unsigned char ch : algorithm) {
        if (ch == '-' || ch == '_' || std::isspace(ch)) {
            continue;
        }
        normalized.push_back(static_cast<char>(std::toupper(ch)));
    }

    return normalized;
}

std::string HashCalculate::toHex(const unsigned char* hash, unsigned int length) {
    std::stringstream ss;
    for (unsigned int i = 0; i < length; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}
