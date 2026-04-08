//
// Реализованы операции с точками на эллиптической кривой: сложение и скалярное умножение.
//

#ifndef ELLIPTICPOINT_H
#define ELLIPTICPOINT_H

#include "BigInteger.h"

class EllipticPoint {
public:
    EllipticPoint();
    EllipticPoint(const BigInteger& x, const BigInteger& y, bool infinity = false);

    EllipticPoint operator+(const EllipticPoint& other) const;
    EllipticPoint operator*(const BigInteger& scalar) const;
    EllipticPoint& operator+=(const EllipticPoint& other);

    bool operator==(const EllipticPoint& other) const;
    bool operator!=(const EllipticPoint& other) const;

    bool isInfinity() const {
        return infinity;
    }
    const BigInteger& getX() const {
        return x;
    }
    const BigInteger& getY() const {
        return y;
    }

    static void setCurveParams(const BigInteger& p, const BigInteger& a, const BigInteger& b, const
        BigInteger& q, const EllipticPoint& generator);

private:
    BigInteger x, y;
    bool infinity;

    static BigInteger p, a, b, q;
    static EllipticPoint generator;
};

#endif //ELLIPTICPOINT_H
