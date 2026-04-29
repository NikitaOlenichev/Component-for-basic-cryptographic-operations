//
// Created by Никита on 21.04.2026.
//

#include "GOSTSignatureInc.h"

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
    BigInteger::fromHex("519E6C5E138ABDCC93191FCECECCD94ADA23BE8B6BBAD133F7BC22D0FB0FBFC248"),
    BigInteger::fromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE7A574A65C5E53C63A5367EBD212EFD"),
    BigInteger::fromHex("88A0BDCDC3F160E646D7689B71E4CED6C97D05137118259AF2066B326966186C"),
    BigInteger::fromHex("B151C4E8BFD2B69A142FEFB638ED31E2CC2DE762C1FB6924B7801C4C2331333F")
);
