//
// Хранение и операции с большими целыми числами, выходящими за рамки стандартных типов C++.
//

#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include <vector>
#include <string>
#include <cstdint>

class BigInteger {
public:
    BigInteger();
    BigInteger(uint64_t value);
    BigInteger(const std::vector<uint8_t>& bytes, bool littleEndian = true);

    std::vector<uint8_t> toBytes(size_t expectedSize = 0) const;
    std::string toString() const;
    std::string toHex() const;

    BigInteger operator+(const BigInteger& other) const;
    BigInteger operator-(const BigInteger& other) const;
    BigInteger operator*(const BigInteger& other) const;
    BigInteger operator/(const BigInteger& other) const;
    BigInteger operator%(const BigInteger& other) const;
    BigInteger abs() const;

    BigInteger& operator+=(const BigInteger& other);
    BigInteger& operator-=(const BigInteger& other);
    BigInteger& operator*=(const BigInteger& other);

    bool operator==(const BigInteger& other) const;
    bool operator!=(const BigInteger& other) const;
    bool operator< (const BigInteger& other) const;
    bool operator> (const BigInteger& other) const;
    bool operator<=(const BigInteger& other) const;
    bool operator>=(const BigInteger& other) const;

    BigInteger modPow(const BigInteger& exponent, const BigInteger& modulus) const;
    BigInteger modInverse(const BigInteger& modulus) const;
    BigInteger modMul(const BigInteger& other, const BigInteger& modulus) const;

    static BigInteger random(const BigInteger& max);
    static BigInteger fromBytes(const std::vector<uint8_t>& bytes, bool littleEndian = true);

    int bitLength() const;
    bool isZero() const;
    void setBit(size_t position, bool value);
    bool getBit(size_t position) const;

    std::vector<uint64_t> data;
    bool isNegative;
};

#endif //BIGINTEGER_H
