//
// Реализованы операции с точками на эллиптической кривой: сложение и скалярное умножение.
// Copyright (c) TheRunuwayDinosaur. All rights reserved.
// (https://github.com/TheRunuwayDinosaur/GOST-R-34.10-12-Elliptic-Curve-Cryptography-/blob/main/sources/h/EllipticPoint.h).
//

#ifndef ELLIPTICPOINTH_H
#define ELLIPTICPOINTH_H

#include "BigInteger.h"

class EllipticPoint{
public:
    EllipticPoint(BigInteger x_ = -1, BigInteger y_ = -1);
    void set_A(const BigInteger &A_);
    void set_B(const BigInteger &B_);
    void set_p(const BigInteger &p_);
    void set_q(const BigInteger &q_);
    void set_mod(const BigInteger &mod_);

    BigInteger get_x() const;
    BigInteger get_y() const;
    BigInteger get_A() const;
    BigInteger get_B() const;
    BigInteger get_p() const;
    BigInteger get_q() const;
    BigInteger get_mod() const;

    void write_point() const;
    bool is_inf() const;

    friend const EllipticPoint operator +(const EllipticPoint &left,const EllipticPoint &right);
    friend const EllipticPoint operator *(EllipticPoint point, BigInteger x);
    friend const BigInteger operator &(const EllipticPoint &left,const EllipticPoint &right);

    EllipticPoint operator +=(const EllipticPoint &right);
    EllipticPoint operator *=(const BigInteger &x);

private:
    BigInteger x;
    BigInteger y;
    BigInteger p;
    BigInteger q;
    BigInteger A;
    BigInteger B;
    BigInteger mod;
};

#endif // ELLIPTICPOINTH_H
