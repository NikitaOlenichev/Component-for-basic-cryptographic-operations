//
// Основная логика УНЭП: генерация ключей, создание и проверка подписи.
// Copyright (c) Andreyinthesky. All rights reserved.
// (https://github.com/Andreyinthesky/cpp-cryptography/blob/master/algorithms/gost_signature.h).
//

#ifndef GOSTSIGNATURE_H
#define GOSTSIGNATURE_H

#include "GOSTSignature.h"
#include "GOSTSignatureInc.h"
#include <vector>
#include <random>
#include <ctime>

class SignaturePrivateKeyProvider {
public:
    virtual BigInteger get_key() = 0;
};

class SignaturePublicKeyProvider {
public:
    virtual ECPoint get_key() = 0;
};

class SignatureProcessor {
public:
    std::vector<uint8_t> generate(std::vector<uint8_t> msg, SignaturePrivateKeyProvider* signature_key_provider) {
        BigInteger key = signature_key_provider->get_key();
        return generate(msg, key);
    }

    std::vector<uint8_t> generate(std::vector<uint8_t> msg, BigInteger signature_key) {
        std::mt19937_64 rng(std::time(nullptr));
        BigInteger k;
        BigInteger e;
        BigInteger r;
        BigInteger s;

        ECPoint P(params.p, params.a, params.b, params.x, params.y);
        e = getHash(msg);

        do {
            k = getRandomNumber(params.q - 1) + 1;
            ECPoint C = P * k;
            r = ModMath::mod(C.x, params.q);
        } while (r == 0);

        s = ModMath::mod(r * signature_key + k * e, params.q);
        if (s == 0)
            return generate(msg, signature_key);

        return make_signature(r, s);
    }

    bool verify(std::vector<uint8_t> msg, std::vector<uint8_t> signature, SignaturePublicKeyProvider* public_key_provider) {
        auto [r, s] = get_r_s(signature);
        if (r <= 0 || r >= params.q || s <= 0 || s >= params.q)
            return false;

        ECPoint P(params.p, params.a, params.b, params.x, params.y);
        BigInteger e = getHash(msg);
        BigInteger v = ModMath::mul_inverse(e, params.q);
        BigInteger z1 = ModMath::mod(s * v, params.q);
        BigInteger z2 = ModMath::mod(-r * v, params.q);
        ECPoint C = P * z1 + public_key_provider->get_key() * z2;

        return ModMath::mod(C.x, params.q) == r;
    }

private:
    SignatureParams params = SignatureParamsSet::CryptoPro_A;
    HashLength hash_len = HashLength::b256;

    BigInteger getRandomNumber(BigInteger max) {
        static std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
        BigInteger result;
        do {
            uint8_t bytes[32];
            for (int i = 0; i < 32; i += 8) {
                uint64_t r = rng();
                for (int j = 0; j < 8; ++j) bytes[i + j] = (r >> (j * 8)) & 0xFF;
            }
            result = BigInteger::from_bytes(std::vector<uint8_t>(bytes, bytes + 32), true);
            result = result % max;
        } while (result == 0);
        return result;
    }

    BigInteger getHash(std::vector<uint8_t> msg) {
        std::vector<uint8_t> hash;
        if (hash_len == b256)
            hash = Stribog::hash256(msg);
        else
            hash = Stribog::hash512(msg);
        BigInteger result(0);
        for (size_t i = 0; i < hash.size(); ++i) {
            result = result * 256 + BigInteger((int)hash[i]);
        }
        return result;
    }

    std::vector<uint8_t> make_signature(BigInteger r, BigInteger s) {
        std::vector<uint8_t> sign_vec;
        // Преобразование r и s в 32-байтные массивы (little-endian)
        auto r_bytes = r.to_bytes(32);
        auto s_bytes = s.to_bytes(32);
        sign_vec.insert(sign_vec.end(), r_bytes.begin(), r_bytes.end());
        sign_vec.insert(sign_vec.end(), s_bytes.begin(), s_bytes.end());
        return sign_vec;
    }

    std::pair<BigInteger, BigInteger> get_r_s(std::vector<uint8_t> signature) {
        if (signature.size() < 64) throw std::runtime_error("Invalid signature size");
        std::vector<uint8_t> r_bytes(signature.begin(), signature.begin() + 32);
        std::vector<uint8_t> s_bytes(signature.begin() + 32, signature.begin() + 64);
        BigInteger r = BigInteger::from_bytes(r_bytes);
        BigInteger s = BigInteger::from_bytes(s_bytes);
        return {r, s};
    }
};


#endif //GOSTSIGNATURE_H
