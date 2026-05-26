//
// Created by Никита on 21.04.2026.
//

#include "Signatures/GOSTSignature.h"

static std::vector<uint8_t> hexBytes(const std::string& hex) {
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        bytes.push_back(static_cast<uint8_t>(std::stoi(hex.substr(i, 2), nullptr, 16)));
    }
    return bytes;
}

SignatureParams SignatureParamsSet::Test = SignatureParams(
    BigInteger("23"), BigInteger("1"), BigInteger("1"),
    BigInteger("29"), BigInteger("2"), BigInteger("3")
); // тестовые параметры

SignatureParams SignatureParamsSet::CryptoPro_A = SignatureParams(
    BigInteger::fromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD97"),
    BigInteger::fromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD94"),
    BigInteger::fromHex("A6"),
    BigInteger::fromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6C611070995AD10045841B09B761B893"),
    BigInteger::fromHex("01"),
    BigInteger::fromHex("8D91E471E0989CDA27DF505A453F2B7635294F2DDF23E3B122ACC99C9E9F1E14")
);
