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
public:
    static BigInteger mul_inverse(BigInteger a, BigInteger n) {
        BigInteger u = mod(a, n);
        BigInteger v = n;
        BigInteger x1 = 1;
        BigInteger x2 = 0;

        while (u != 1 && v != 1) {
            while (u.isEven()) {
                u.divide_by_2();
                if (x1.isEven()) {
                    x1.divide_by_2();
                } else {
                    x1 = (x1 + n);
                    x1.divide_by_2();
                }
            }
            while (v.isEven()) {
                v.divide_by_2();
                if (x2.isEven()) {
                    x2.divide_by_2();
                } else {
                    x2 = (x2 + n);
                    x2.divide_by_2();
                }
            }
            if (u >= v) {
                u = u - v;
                x1 = x1 - x2;
            } else {
                v = v - u;
                x2 = x2 - x1;
            }
        }
        BigInteger res = (u == 1) ? x1 : x2;
        return mod(res, n);
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

    ECPoint operator*(const BigInteger& n) const {
        if (n == 0) return ECPoint(p, a, b, 0, 0);
        ECPoint result = ECPoint(p, a, b, 0, 0);
        ECPoint base = *this;
        BigInteger exp = n;
        while (exp != 0) {
            if (exp.isOdd()) {
                result = result + base;
            }
            base = base + base;
            exp.divide_by_2();
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
