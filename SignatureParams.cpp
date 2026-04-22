//
// Created by Никита on 21.04.2026.
//

#include "GOSTSignatureInc.h"

SignatureParams SignatureParamsSet::Test = SignatureParams(
    BigInteger("23"), BigInteger("1"), BigInteger("1"),
    BigInteger("29"), BigInteger("2"), BigInteger("3")
); // тестовые параметры

SignatureParams SignatureParamsSet::CryptoPro_A = SignatureParams(
    BigInteger("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD"),
    BigInteger("3"),
    BigInteger("519E6C5E138ABDCC93191FCECECCD94ADA23BE8B6BBAD133F7BC22D0FBFC248"),
    BigInteger("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE7A574A65C5E53C63A5367EBD212EFD"),
    BigInteger("88A0BDCDC3F160E646D7689B71E4CED6C97D05137118259AF2066B326966186C"),
    BigInteger("B151C4E8BFD2B69A142FEFB638ED31E2CC2DE762C1FB6924B7801C4C2331333F")
);
