//
// Реализованы операции с точками на эллиптической кривой: сложение и скалярное умножение.
//

#include "EllipticPoint.h"
#include <stdexcept>

BigInteger EllipticPoint::p, EllipticPoint::a, EllipticPoint::b, EllipticPoint::q;
EllipticPoint EllipticPoint::generator;

EllipticPoint::EllipticPoint() : infinity(true) {}

EllipticPoint::EllipticPoint(const BigInteger &x, const BigInteger &y, bool infinity) : x(x), y(y), infinity(infinity) {}

void EllipticPoint::setCurveParams(const BigInteger &p_, const BigInteger &a_, const BigInteger &b_, const BigInteger &q_, const EllipticPoint &generator_) {
    p = p_;
    a = a_;
    b = b_;
    q = q_;
    generator = generator_;
}

EllipticPoint EllipticPoint::operator+(const EllipticPoint &other) const {
    if (infinity) return other;
    if (other.infinity) return *this;
    if (*this == -other) return EllipticPoint();

    BigInteger lambda;
    if (*this == other) {
        BigInteger numerator = (x * x * 3 + a) % p;
        BigInteger denominator = (y * 2) % p;
        lambda = numerator * denominator.modInverse(p) % p;
    } else {
        BigInteger numerator = (other.y - y) % p;
        BigInteger denominator = (other.x - x) % p;
        lambda = numerator * denominator.modInverse(p) % p;
    }

    BigInteger x3 = (lambda * lambda - x - other.x) % p;
    BigInteger y3 = (lambda * (x - x3) - y) % p;

    if (x3.isNegative) x3 += p;
    if (y3.isNegative) y3 += p;

    return EllipticPoint(x3, y3);
}

EllipticPoint EllipticPoint::operator*(const BigInteger &scalar) const {
    if (scalar.isZero() || infinity) return EllipticPoint();

    EllipticPoint result;
    EllipticPoint base = *this;
    BigInteger exp = scalar;

    while (!exp.isZero()) {
        if (exp.data[0] & 1) {
            result = result + base;
        }
        base = base + base;
        uint64_t carry = 0;
        for (int i = exp.data.size() - 1; i >= 0; i--) {
            uint64_t current = (carry << 64) | exp.data[i];
            exp.data[i] = current >> 1;
            carry = current & 1;
        }
        if (exp.data.size() > 1 && exp.data.back() == 0) {
            exp.data.pop_back();
        }
    }

    return result;
}

EllipticPoint &EllipticPoint::operator+=(const EllipticPoint &other) {
    *this = *this + other;
    return *this;
}

bool EllipticPoint::operator==(const EllipticPoint &other) const {
    if (infinity && other.infinity) return true;
    if (infinity || other.infinity) return false;
    return x == other.x && y == other.y;
}

bool EllipticPoint::operator!=(const EllipticPoint &other) const {
    return !(*this == other);
}

