//
// Реализованы операции с точками на эллиптической кривой: сложение и скалярное умножение.
// Copyright (c) TheRunuwayDinosaur. All rights reserved.
// (https://github.com/TheRunuwayDinosaur/GOST-R-34.10-12-Elliptic-Curve-Cryptography-/blob/main/sources/cpp/EllipticPoint.cpp).
//

#include "EllipticPoint.h"
#include <iostream>

EllipticPoint::EllipticPoint(BigInteger x_, BigInteger y_){
    this->x = x_;
    this->y = y_;
}

void EllipticPoint::set_A(const BigInteger &A_){
    this->A = A_;
}

void EllipticPoint::set_B(const BigInteger &B_){
    this->B = B_;
}

void EllipticPoint::set_p(const BigInteger &p_){
    this->p = p_;
}

void EllipticPoint::set_q(const BigInteger &q_) {
    this->q = q_;
}

void EllipticPoint::set_mod(const BigInteger &mod_) {
    this->mod = mod_;
}

BigInteger EllipticPoint::get_x() const{
    return this->x;
}

BigInteger EllipticPoint::get_y() const{
    return this->y;
}

BigInteger EllipticPoint::get_A() const{
    return this->A;
}

BigInteger EllipticPoint::get_B() const{
    return this->B;
}

BigInteger EllipticPoint::get_p() const{
    return this->p;
}

BigInteger EllipticPoint::get_q() const {
    return this->q;
}

BigInteger EllipticPoint::get_mod() const {
    return this->mod;
}

void EllipticPoint::write_point() const {
    std::cout << "(" << this->x.toString() << ", " << this->y.toString() << ")" << std::endl;
}

bool EllipticPoint::is_inf() const{
    return this->x == 0 && this->y == 0;
}

EllipticPoint EllipticPoint::operator +=(const EllipticPoint &right){
    return *this = *this + right;
}

EllipticPoint EllipticPoint::operator *=(const BigInteger &x){
    return *this = *this * x;
}
