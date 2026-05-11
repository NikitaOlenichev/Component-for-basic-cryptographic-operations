//
// Хранение и операции с большими целыми числами, выходящими за рамки стандартных типов C++.
// Copyright (c) TheRunuwayDinosaur. All rights reserved.
// (https://github.com/TheRunuwayDinosaur/GOST-R-34.10-12-Elliptic-Curve-Cryptography-/blob/main/sources/h/BigInteger.h).
//

#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

class BigInteger{
public:
    BigInteger();
    BigInteger(std::string str);
    BigInteger(int x);
    BigInteger(unsigned int x);
    BigInteger(long long x);
    BigInteger(unsigned long long x);

    void set_number(std::string str);
    void shift_right();
    bool isOdd() const;
    bool isEven() const;
    int getLowestBit() const;
    void remove_leading_zeros();
    void divide_by_2();

    std::string toString() const;
    int log2() const;

    BigInteger operator +() const;
    BigInteger operator -() const;

    friend std::ostream& operator <<(std::ostream& os, const BigInteger& bi);
    friend bool operator == (const BigInteger& left, const BigInteger& right);
    friend bool operator < (const BigInteger& left, const BigInteger& right);
    friend bool operator !=(const BigInteger& left, const BigInteger& right);
    friend bool operator <=(const BigInteger& left, const BigInteger& right);
    friend bool operator > (const BigInteger& left, const BigInteger& right);
    friend bool operator >= (const BigInteger& left, const BigInteger& right);

    friend const BigInteger operator +(BigInteger left, const BigInteger& right);
    friend const BigInteger operator -(BigInteger left, const BigInteger& right);
    friend const BigInteger operator *(const BigInteger& left, const BigInteger& right);
    friend const BigInteger operator /(const BigInteger& left, const BigInteger& right);
    friend const BigInteger operator %(const BigInteger& left, const BigInteger& right);

    BigInteger operator +=(const BigInteger &value);
    BigInteger operator -=(const BigInteger &value);
    BigInteger operator *= (const BigInteger &value);
    BigInteger operator /= (const BigInteger &value);
    BigInteger operator %= (const BigInteger &value);

    BigInteger operator ++();
    BigInteger operator ++(int);
    BigInteger operator --();
    BigInteger operator --(int);

    int to_int() const;
    BigInteger operator<<(int shift) const;
    std::vector<uint8_t> to_bytes(size_t size = 0) const;
    static BigInteger from_bytes(const std::vector<uint8_t>& bytes, bool little_endian = true);
    static BigInteger fromHex(const std::string& hex);
    std::string toHex() const;

private:
    static const long long BASE = 1000000000;
    std::vector <int> numbers;
    bool isNegative;
};

#endif // BIGINTEGER_H
