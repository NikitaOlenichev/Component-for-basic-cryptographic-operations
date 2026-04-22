//
// Основная логика УНЭП: генерация ключей, создание и проверка подписи.
// Copyright (c) Andreyinthesky. All rights reserved.
// (https://github.com/Andreyinthesky/cpp-cryptography/blob/master/algorithms/gost_signature_inc.h).
//

#ifndef GOSTSIGNATUREINC_H
#define GOSTSIGNATUREINC_H

#include "BigInteger.h"
#include "Stribog.h"
#include <stdexcept>
#include <tuple>

class ModMath {
private:
    static std::tuple<BigInteger, BigInteger, BigInteger> ext_euclidean_algorithm(BigInteger a, BigInteger b) {
        BigInteger s = 0;
        BigInteger old_s = 1;
        BigInteger t = 1;
        BigInteger old_t = 0;
        BigInteger r = b;
        BigInteger old_r = a;
        while (r != 0) {
            BigInteger quotient = old_r / r;
            BigInteger temp = r;
            r = old_r - quotient * r;
            old_r = temp;

            temp = s;
            s = old_s - quotient * s;
            old_s = temp;

            temp = t;
            t = old_t - quotient * t;
            old_t = temp;
        }
        return std::make_tuple(old_r, old_s, old_t);
    }

public:
    static BigInteger mul_inverse(BigInteger n, BigInteger p) {
        if (n < 0)
            return p - mul_inverse(-n, p);
        auto [gcd, x, y] = ext_euclidean_algorithm(n, p);
        if (gcd != 1)
            throw std::domain_error("has no multiplicative inverse");
        return mod(x, p);
    }

    static BigInteger mod(BigInteger n, BigInteger p) {
        BigInteger r = n % p;
        if (r < 0)
            r = r + p;
        return r;
    }
};

struct ECPoint {
public:
    BigInteger a;
    BigInteger b;
    BigInteger x;
    BigInteger y;
    BigInteger p;

    static bool isZero(const ECPoint& p) {
        return p.x == 0 && p.y == 0;
    }

    ECPoint(BigInteger p_, BigInteger a_, BigInteger b_, BigInteger x_, BigInteger y_)
        : p(p_), a(a_), b(b_), x(x_), y(y_) {}

    bool operator==(const ECPoint& other) const {
        return x == other.x && y == other.y;
    }

    ECPoint operator+(const ECPoint& other) const {
        if (isZero(*this)) return other;
        if (isZero(other)) return *this;
        if (x == other.x && y != other.y)
            return ECPoint(p, a, b, 0, 0);
        BigInteger m;
        if (*this == other) {
            BigInteger numerator = (x * x * 3 + a) % p;
            BigInteger denominator = (y * 2) % p;
            m = numerator * ModMath::mul_inverse(denominator, p) % p;
        } else {
            BigInteger numerator = (other.y - y) % p;
            if (numerator < 0) numerator += p;
            BigInteger denominator = (other.x - x) % p;
            if (denominator < 0) denominator += p;
            m = numerator * ModMath::mul_inverse(denominator, p) % p;
        }
        BigInteger x3 = (m * m - x - other.x) % p;
        BigInteger y3 = (m * (x - x3) - y) % p;
        if (x3 < 0) x3 += p;
        if (y3 < 0) y3 += p;
        return ECPoint(p, a, b, x3, y3);
    }

    ECPoint operator*(BigInteger n) const {
        ECPoint result = ECPoint(p, a, b, 0, 0);
        ECPoint point = *this;
        while (n != 0) {
            if (n.odd())
                result = result + point;
            point = point + point;
            n = n / 2;
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& s, const ECPoint& point) {
        s << "(x: " << point.x.toString() << ", y: " << point.y.toString() << ")";
        return s;
    }
};

class SignatureParams {
public:
    BigInteger p;
    BigInteger a;
    BigInteger b;
    BigInteger q;
    BigInteger x;
    BigInteger y;

    SignatureParams(BigInteger p_, BigInteger a_, BigInteger b_, BigInteger q_, BigInteger x_, BigInteger y_)
        : p(p_), a(a_), b(b_), q(q_), x(x_), y(y_) {}
};

struct SignatureParamsSet {
    static SignatureParams Test;
    static SignatureParams CryptoPro_A;
};


#endif //GOSTSIGNATUREINC_H
