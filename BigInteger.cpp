//
// Хранение и операции с большими целыми числами, выходящими за рамки стандартных типов C++.
//

#include "BigInteger.h"
#include <algorithm>
#include <stdexcept>
#include <random>
#include <iomanip>
#include <sstream>

BigInteger::BigInteger() : isNegative(false) {
    data.push_back(0);
}

BigInteger::BigInteger(uint64_t value) : isNegative(false) {
    if (value == 0) {
        data.push_back(0);
    } else {
        while (value > 0) {
            data.push_back(value & 0xFFFFFFFFFFFFFFFFULL);
            value >>= 64;
        }
    }
}

BigInteger::BigInteger(const std::vector<uint8_t> &bytes, bool littleEndian) : isNegative(false) {
    if (bytes.empty()) {
        data.push_back(0);
        return;
    }

    size_t numWords = (bytes.size() + 7) / 8;
    data.resize(numWords, 0);

    if (littleEndian) {
        for (size_t i = 0; i < bytes.size(); i++) {
            size_t wordIndex = i / 8;
            size_t byteOffset = i % 8;
            data[wordIndex] |= (static_cast<uint64_t>(bytes[i]) << (byteOffset * 8));
        }
    } else {
        for (size_t i = 0; i < bytes.size(); i++) {
            size_t wordIndex = (bytes.size() - 1 - i) / 8;
            size_t byteOffset = (bytes.size() - 1 - i) % 8;
            data[wordIndex] |= (static_cast<uint64_t>(bytes[i]) << (byteOffset * 8));
        }
    }

    while (data.size() > 1 && data.back() == 0) {
        data.pop_back();
    }
}

std::vector<uint8_t> BigInteger::toBytes(size_t expectedSize) const {
    std::vector<uint8_t> result;
    size_t numBytes = (bitLength() + 7) / 8;
    if (expectedSize > 0 && expectedSize > numBytes) {
        numBytes = expectedSize;
    }

    result.resize(numBytes, 0);

    for (size_t i = 0; i < data.size() && i * 8 < numBytes; i++) {
        uint64_t word = data[i];
        for (size_t j = 0; j < 8 && i * 8 + j < numBytes; j++) {
            result[i * 8 + j] = (word >> (j * 8)) & 0xFF;
        }
    }

    return result;
}

std::string BigInteger::toString() const {
    if (isZero()) return "0";

    BigInteger temp = *this;
    temp.isNegative = false;
    std::string result;

    while (!temp.isZero()) {
        uint64_t remainder = 0;
        for (int i = temp.data.size() - 1; i >= 0; i--) {
            uint64_t current = remainder * 0x10000000000000000ULL + temp.data[i];
            temp.data[i] = current / 10;
            remainder = current % 10;
        }
        result.push_back('0' + remainder);
        while (temp.data.size() > 1 && temp.data.back() == 0) {
            temp.data.pop_back();
        }
    }

    if (isNegative) result.push_back('-');
    std::reverse(result.begin(), result.end());
    return result;
}

std::string BigInteger::toHex() const {
    if (isZero()) return "0";

    std::stringstream ss;
    if (isNegative) ss << "-";
    for (int i = data.size() - 1; i >= 0; i--) {
        ss << std::hex << std::setfill('0') << std::setw(16) << data[i];
    }
    std::string result = ss.str();
    size_t pos = result.find_first_not_of('0');
    if (pos != std::string::npos) {
        result = result.substr(pos);
    }
    return result;
}

BigInteger BigInteger::abs() const {
    BigInteger result = *this;
    result.isNegative = false;
    return result;
}

BigInteger BigInteger::operator+(const BigInteger &other) const {
    if (isNegative != other.isNegative) {
        if (isNegative) return other - (-(*this));
        else return *this - (-other);
    }

    BigInteger result;
    result.isNegative = isNegative;
    result.data.clear();

    uint64_t carry = 0;
    size_t maxSize = std::max(data.size(), other.data.size());
    for (size_t i = 0; i < maxSize || carry; i++) {
        uint64_t sum = carry;
        if (i < data.size()) sum += data[i];
        if (i < other.data.size()) sum += other.data[i];
        result.data.push_back(sum);
        carry = (sum < data[i] || sum < other.data[i]) ? 1 : 0;
    }

    return result;
}

BigInteger BigInteger::operator-(const BigInteger &other) const {
    if (isNegative != other.isNegative) {
        return *this + (-other);
    }

    if (abs() < other.abs()) {
        return -(other - *this);
    }

    BigInteger result;
    result.isNegative = false;
    result.data.clear();

    uint64_t borrow = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        uint64_t subtrahend = borrow;
        if (i < other.data.size()) subtrahend += other.data[i];
        uint64_t diff;
        if (data[i] >= subtrahend) {
            diff = data[i] - subtrahend;
            borrow = 0;
        } else {
            diff = data[i] + (0x10000000000000000ULL - subtrahend);
            borrow = 1;
        }
        result.data.push_back(diff);
    }

    while (result.data.size() > 1 && result.data.back() == 0) {
        result.data.pop_back();
    }

    return result;
}

BigInteger BigInteger::operator*(const BigInteger &other) const {
    if (isZero() || other.isZero()) return BigInteger();

    BigInteger result;
    result.data.resize(data.size() + other.data.size(), 0);

    for (size_t i = 0; i < data.size(); i++) {
        uint64_t carry = 0;
        for (size_t j = 0; j < other.data.size() || carry; j++) {
            uint64_t sum = result.data[i + j] + carry;
            if (j < other.data.size()) {
                sum += data[i] * other.data[j];
            }
            result.data[i + j] = sum;
            carry = sum >> 64;
        }
    }

    while (result.data.size() > 1 && result.data.back() == 0) {
        result.data.pop_back();
    }

    result.isNegative = isNegative != other.isNegative;
    return result;
}

static void divide(const BigInteger& a, const BigInteger& b, BigInteger& quotient, BigInteger& remainder) {
    if (b.isZero()) throw std::runtime_error("Division by zero");

    quotient = BigInteger();
    remainder = a;
    remainder.isNegative = false;

    BigInteger divisor = b;
    divisor.isNegative = false;

    if (remainder < divisor) return;

    int shift = remainder.bitLength() - divisor.bitLength();
    divisor <<= shift;
    quotient <<= shift;

    while (shift >= 0) {
        if (remainder >= divisor) {
            remainder -= divisor;
            quotient.setBit(shift, true);
        }
        divisor >>= 1;
        shift--;
    }

    quotient.isNegative = a.isNegative != b.isNegative;
    remainder.isNegative = a.isNegative;
}

BigInteger BigInteger::operator/(const BigInteger &other) const {
    BigInteger quotient, remainder;
    divide(*this, other, quotient, remainder);
    return quotient;
}

BigInteger BigInteger::operator%(const BigInteger &other) const {
    BigInteger quotient, remainder;
    divide(*this, other, quotient, remainder);
    return remainder;
}

BigInteger &BigInteger::operator+=(const BigInteger &other) {
    *this = *this + other;
    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {
    *this = *this - other;
    return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& other) {
    *this = *this * other;
    return *this;
}

bool BigInteger::operator==(const BigInteger &other) const {
    if (isNegative != other.isNegative) return false;
    if (data.size() != other.data.size()) return false;
    for (size_t i = 0; i < data.size(); i++) {
        if (data[i] != other.data[i]) return false;
    }
    return true;
}

bool BigInteger::operator!=(const BigInteger& other) const {
    return !(*this == other);
}

bool BigInteger::operator<(const BigInteger &other) const {
    if (isNegative != other.isNegative) return isNegative;
    if (isNegative) return (-other) < (-*this);
    if (data.size() != other.data.size()) return data.size() < other.data.size();
    for (int i = data.size() - 1; i >= 0; i--) {
        if (data[i] != other.data[i]) return data[i] < other.data[i];
    }
    return false;
}

bool BigInteger::operator>(const BigInteger& other) const {
    return other < *this;
}

bool BigInteger::operator<=(const BigInteger& other) const {
    return !(*this > other);
}

bool BigInteger::operator>=(const BigInteger& other) const {
    return !(*this < other);
}

BigInteger BigInteger::modPow(const BigInteger &exponent, const BigInteger &modulus) const {
    if (modulus.isZero()) throw std::runtime_error("Modulo is zero");
    if (modulus == BigInteger(1)) return BigInteger();

    BigInteger result(1);
    BigInteger base = *this % modulus;
    BigInteger exp = exponent;

    while (!exp.isZero()) {
        if (exp.data[0] & 1) {
            result = (result * base) % modulus;
        }
        base = (base * base) % modulus;
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

BigInteger BigInteger::modInverse(const BigInteger &modulus) const {
    if (modulus.isZero()) throw std::runtime_error("Modulo is zero");

    BigInteger a = *this % modulus;
    BigInteger b = modulus;
    BigInteger x0(1), x1(0);

    while (!b.isZero()) {
        BigInteger q = a / b;
        BigInteger r = a % b;
        a = b;
        b = r;
        BigInteger x = x0 - q * x1;
        x0 = x1;
        x1 = x;
    }

    if (a != BigInteger(1)) {
        throw std::runtime_error("Inverse does not exist");
    }

    if (x0 < 0) {
        x0 += modulus;
    }

    return x0;
}

BigInteger BigInteger::modMul(const BigInteger &other, const BigInteger &modulus) const {
    return ((*this) * other) % modulus;
}

BigInteger BigInteger::random(const BigInteger &max) {
    if (max <= BigInteger(1)) return BigInteger();

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dist;

    BigInteger result;
    result.data.resize(max.data.size());

    do {
        for (size_t i = 0; i < max.data.size(); i++) {
            result.data[i] = dist(gen);
        }
        while (result.data.size() > 1 && result.data.back() == 0) {
            result.data.pop_back();
        }
    } while (result >= max);

    return result;
}

BigInteger BigInteger::fromBytes(const std::vector<uint8_t> &bytes, bool littleEndian) {
    return BigInteger(bytes, littleEndian);
}

int BigInteger::bitLength() const {
    if (isZero()) return 0;
    int bits = (data.size() - 1) * 64;
    uint64_t lastWord = data.back();
    while (lastWord) {
        bits++;
        lastWord >>= 1;
    }
    return bits;
}

bool BigInteger::isZero() const {
    return data.size() == 1 && data[0] == 0 && !isNegative;
}

void BigInteger::setBit(size_t position, bool value) {
    size_t wordIndex = position / 64;
    size_t bitIndex = position % 64;
    if (wordIndex >= data.size()) {
        data.resize(wordIndex + 1, 0);
    }
    if (value) {
        data[wordIndex] |= (1ULL << bitIndex);
    } else {
        data[wordIndex] &= ~(1ULL << bitIndex);
    }
    while (data.size() > 1 && data.back() == 0) {
        data.pop_back();
    }
}

bool BigInteger::getBit(size_t position) const {
    size_t wordIndex = position / 64;
    size_t bitIndex = position % 64;
    if (wordIndex >= data.size()) return false;
    return (data[wordIndex] >> bitIndex) & 1;
}

