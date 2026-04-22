//
// Хранение и операции с большими целыми числами, выходящими за рамки стандартных типов C++.
// Copyright (c) TheRunuwayDinosaur. All rights reserved.
// (https://github.com/TheRunuwayDinosaur/GOST-R-34.10-12-Elliptic-Curve-Cryptography-/blob/main/sources/cpp/BigInteger.cpp).
//

#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <stdexcept>

#include "BigInteger.h"

BigInteger::BigInteger(){
    this->isNegative = false;
}

BigInteger::BigInteger(std::string str){
    this->set_number(str);
}

BigInteger::BigInteger(int x){
    if (x < 0) {
        this->isNegative = true;
        x *= -1;
    }
    else this->isNegative = false;
    do{
        this->numbers.push_back(x % BigInteger::BASE);
        x /= BigInteger::BASE;
    } while (x != 0);
}

BigInteger::BigInteger(unsigned int x){
    this->isNegative = false;
    do {
        this->numbers.push_back(x % BigInteger::BASE);
        x /= BigInteger::BASE;
    } while (x != 0);
}

BigInteger::BigInteger(long long x){
    if (x < 0) {
        this->isNegative = true;
        x *= -1;
    }
    else this->isNegative = false;
    do{
        this->numbers.push_back(x % BigInteger::BASE);
        x /= BigInteger::BASE;
    } while (x != 0);
}

BigInteger::BigInteger(unsigned long long x){
    this->isNegative = false;
    do {
        this->numbers.push_back(x % BigInteger::BASE);
        x /= BigInteger::BASE;
    } while (x != 0);
}

void BigInteger::set_number(std::string str){
    this->numbers.clear();
    if (str.size() == 0) {
        this->isNegative = false;
    }
    else{
        if (str[0] == '-') {
            str = str.substr(1);
            this->isNegative = true;
        }
        else this->isNegative = false;
        for (int i = str.size(); i > 0; i -= 9) {
            if (i < 9)
                this->numbers.push_back(atoi(str.substr(0, i).c_str()));
            else
                this->numbers.push_back(atoi(str.substr(i - 9, 9).c_str()));
        }
        this->remove_leading_zeros();
    }
}

void BigInteger::shift_right(){
    if (this->numbers.size() == 0) {
        this->numbers.push_back(0);
        return;
    }
    this->numbers.push_back(this->numbers[this->numbers.size() - 1]);
    for (int i = this->numbers.size() - 2; i > 0; --i)
        this->numbers[i] = this->numbers[i - 1];
    this->numbers[0] = 0;
}

bool BigInteger::odd(){
    if (this->numbers.size() == 0) return false;
    return this->numbers[0] & 1;
}

bool BigInteger::even(){
    return !this->odd();
}

void BigInteger::remove_leading_zeros(){
    while (this->numbers.size() > 1 && !this->numbers.back()) this->numbers.pop_back();
    if (this->numbers.size() == 1 && this->numbers[0] == 0) this->isNegative = false;
}

void BigInteger::divide_by_2(){
    int add = 0;
    for (int i = this->numbers.size() - 1;i >= 0;i--){
        int digit = (this->numbers[i] >> 1) + add;
        add = ((this->numbers[i] & 1) * (this->BASE / 2));
        this->numbers[i] = digit;
    }
    this->remove_leading_zeros();
}

std::string BigInteger::toString() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

int BigInteger::log2() const{
    BigInteger temp = *this;
    int log = 0;
    while(temp > 0){
        log++; temp.divide_by_2();
    }
    return log;
}

BigInteger BigInteger::operator +() const{
    return BigInteger(*this);
}

BigInteger BigInteger::operator -() const{
    BigInteger result = *this;
    if (result != 0)
        result.isNegative = !result.isNegative;
    return result;
}

std::ostream& operator <<(std::ostream& os, const BigInteger& bi) {
    if (bi.isNegative)
        os << '-';
    os << bi.numbers.back();
    for (int i = (int)bi.numbers.size() - 2; i >= 0; --i)
        os << std::setw(9) << std::setfill('0') << bi.numbers[i];
    return os;
}

bool operator == (const BigInteger& left, const BigInteger& right) {
    if (left.isNegative != right.isNegative)
        return false;
    if (left.numbers.size() != right.numbers.size())
        return false;
    for (size_t i = 0; i < left.numbers.size(); ++i)
        if (left.numbers[i] != right.numbers[i])
            return false;
    return true;
}

bool operator < (const BigInteger& left, const BigInteger& right) {
    if (left == right)
        return false;
    if (left.isNegative != right.isNegative)
        return left.isNegative;
    if (left.isNegative)
        return (-right) < (-left);
    if (left.numbers.size() != right.numbers.size())
        return left.numbers.size() < right.numbers.size();
    for (int i = left.numbers.size() - 1; i >= 0; --i)
        if (left.numbers[i] != right.numbers[i])
            return left.numbers[i] < right.numbers[i];
    return false;
}

bool operator !=(const BigInteger& left, const BigInteger& right) {
    return !(left == right);
}

bool operator <=(const BigInteger& left, const BigInteger& right) {
    return (left < right) || (left == right);
}

bool operator > (const BigInteger& left, const BigInteger& right) {
    return !(left <= right);
}

bool operator >= (const BigInteger& left, const BigInteger& right) {
    return !(left < right);
}

const BigInteger operator +(BigInteger left, const BigInteger& right) {
    if (left.isNegative == right.isNegative) {
        int carry = 0;
        for (size_t i = 0; i < std::max(left.numbers.size(), right.numbers.size()) || carry; ++i) {
            if (i == left.numbers.size())
                left.numbers.push_back(0);
            left.numbers[i] += carry + (i < right.numbers.size() ? right.numbers[i] : 0);
            carry = left.numbers[i] >= BigInteger::BASE;
            if (carry)
                left.numbers[i] -= BigInteger::BASE;
        }
        return left;
    }
    return left - (-right);
}

const BigInteger operator -(BigInteger left, const BigInteger& right) {
    if (right.isNegative)
        return left + (-right);
    if (left.isNegative)
        return -((-left) + right);
    if (left < right)
        return -(right - left);
    int borrow = 0;
    for (size_t i = 0; i < left.numbers.size(); ++i) {
        left.numbers[i] -= borrow + (i < right.numbers.size() ? right.numbers[i] : 0);
        borrow = left.numbers[i] < 0;
        if (borrow)
            left.numbers[i] += BigInteger::BASE;
    }
    left.remove_leading_zeros();
    return left;
}

const BigInteger operator *(const BigInteger& left, const BigInteger& right) {
    BigInteger result;
    result.numbers.resize(left.numbers.size() + right.numbers.size(), 0);
    for (size_t i = 0; i < left.numbers.size(); ++i) {
        int carry = 0;
        for (size_t j = 0; j < right.numbers.size() || carry; ++j) {
            long long cur = result.numbers[i + j] + carry +
                (j < right.numbers.size() ? (long long)left.numbers[i] * right.numbers[j] : 0);
            result.numbers[i + j] = (int)(cur % BigInteger::BASE);
            carry = (int)(cur / BigInteger::BASE);
        }
    }
    result.remove_leading_zeros();
    result.isNegative = left.isNegative != right.isNegative;
    return result;
}

const BigInteger operator /(const BigInteger& left, const BigInteger& right) {
    if (right == 0)
        throw std::runtime_error("Division by zero");
    BigInteger result;
    result.isNegative = left.isNegative != right.isNegative;
    BigInteger current;
    current.isNegative = false;
    for (int i = left.numbers.size() - 1; i >= 0; --i) {
        current.numbers.insert(current.numbers.begin(), left.numbers[i]);
        current.remove_leading_zeros();
        int x = 0;
        int l = 0;
        int r = BigInteger::BASE;
        while (l <= r) {
            int m = (l + r) / 2;
            BigInteger t = right * BigInteger(m);
            t.isNegative = false;
            if (t <= current) {
                x = m;
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        result.numbers.insert(result.numbers.begin(), x);
        current = current - right * BigInteger(x);
    }
    result.remove_leading_zeros();
    return result;
}

const BigInteger operator %(const BigInteger& left, const BigInteger& right) {
    return left - (left / right) * right;
}

BigInteger BigInteger::operator +=(const BigInteger &value){
    return *this = (*this + value);
}

BigInteger BigInteger::operator -=(const BigInteger &value){
    return *this = (*this - value);
}

BigInteger BigInteger::operator *=(const BigInteger &value){
    return *this = (*this * value);
}

BigInteger BigInteger::operator /=(const BigInteger &value){
    return *this = (*this / value);
}

BigInteger BigInteger::operator %=(const BigInteger &value){
    return *this = (*this % value);
}

BigInteger BigInteger::operator ++(){
    return (*this += 1);
}

BigInteger BigInteger::operator --(){
    return (*this -= 1);
}

BigInteger BigInteger::operator ++(int){
    *this += 1;
    return *this - 1;
}

BigInteger BigInteger::operator --(int){
    *this -= 1;
    return *this + 1;
}

int BigInteger::to_int() const {
    if (numbers.empty()) return 0;
    return numbers[0];
}

BigInteger BigInteger::operator<<(int shift) const {
    BigInteger result = *this;
    for (int i = 0; i < shift; ++i)
        result = result * 2;
    return result;
}

std::vector<uint8_t> BigInteger::to_bytes(size_t size) const {
    std::vector<uint8_t> result;
    BigInteger temp = *this;
    while (temp != 0) {
        result.push_back((temp % 256).to_int());
        temp = temp / 256;
    }
    if (result.empty()) result.push_back(0);
    if (size > result.size()) result.resize(size, 0);
    return result;
}

BigInteger BigInteger::from_bytes(const std::vector<uint8_t>& bytes, bool little_endian) {
    BigInteger result(0);
    if (little_endian) {
        for (size_t i = 0; i < bytes.size(); ++i) {
            result = result + BigInteger(bytes[i]) * (BigInteger(1) << (i * 8));
        }
    } else {
        for (size_t i = 0; i < bytes.size(); ++i) {
            result = result * 256 + BigInteger(bytes[i]);
        }
    }
    return result;
}
