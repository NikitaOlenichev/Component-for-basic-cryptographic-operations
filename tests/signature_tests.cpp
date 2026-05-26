#include <gtest/gtest.h>

#include "Signatures/BigInteger.h"
#include "Signatures/CAdESBuilder.h"
#include "Signatures/GOSTSignature.h"
#include "Signatures/GOSTSignatureInc.h"
#include "Signatures/Stribog.h"

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
class StaticPublicKeyProvider : public SignaturePublicKeyProvider {
public:
    explicit StaticPublicKeyProvider(const ECPoint& key) : key_(key) {}

    ECPoint get_key() override {
        return key_;
    }

private:
    ECPoint key_;
};
}

// ================== BigInteger ==================
TEST(BigIntegerTest, ArithmeticAndComparison) {
    const BigInteger a(500);
    const BigInteger b(200);

    EXPECT_EQ(std::string("700"), (a + b).toString());
    EXPECT_EQ(std::string("300"), (a - b).toString());
    EXPECT_EQ(std::string("100000"), (a * b).toString());
    EXPECT_EQ(std::string("2"), (a / b).toString());
    EXPECT_EQ(std::string("100"), (a % b).toString());
    EXPECT_TRUE(a > b);
    EXPECT_TRUE(b < a);
}

TEST(BigIntegerTest, BytesAndHexRoundTrip) {
    const BigInteger value = BigInteger::fromHex("01020304");
    const std::vector<uint8_t> bytes = value.to_bytes(4);

    EXPECT_EQ(static_cast<size_t>(4), bytes.size());
    EXPECT_EQ(value.toString(), BigInteger::from_bytes(bytes).toString());
    EXPECT_EQ(value.toString(), BigInteger::fromHex(value.toHex()).toString());
}

TEST(BigIntegerTest, BitHelpers) {
    BigInteger value(8);

    EXPECT_TRUE(value.isEven());
    EXPECT_FALSE(value.isOdd());
    value.divide_by_2();
    EXPECT_EQ(std::string("4"), value.toString());
}

TEST(BigIntegerTest, MutatingOperatorsAndConversions) {
    BigInteger value(10);

    value += BigInteger(5);
    EXPECT_EQ(std::string("15"), value.toString());
    value -= BigInteger(3);
    EXPECT_EQ(std::string("12"), value.toString());
    value *= BigInteger(2);
    EXPECT_EQ(std::string("24"), value.toString());
    value /= BigInteger(4);
    EXPECT_EQ(std::string("6"), value.toString());
    value %= BigInteger(4);
    EXPECT_EQ(std::string("2"), value.toString());

    EXPECT_EQ(2, value.to_int());
    EXPECT_EQ(std::string("4"), (value << 1).toString());
    EXPECT_EQ(std::string("3"), (++value).toString());
    EXPECT_EQ(std::string("3"), (value++).toString());
    EXPECT_EQ(std::string("4"), value.toString());
    EXPECT_EQ(std::string("3"), (--value).toString());
    EXPECT_EQ(std::string("3"), (value--).toString());
    EXPECT_EQ(std::string("2"), value.toString());
}

TEST(BigIntegerTest, NegativeNumbersAndSetNumber) {
    BigInteger negative(-5);
    BigInteger value;

    value.set_number("-12");
    EXPECT_EQ(std::string("-12"), value.toString());
    EXPECT_EQ(std::string("-7"), (value - negative).toString());
    EXPECT_TRUE(value < BigInteger(0));
    EXPECT_EQ(std::string("5"), (-negative).toString());
}

// ================== Stribog ==================
TEST(StribogTest, StaticHashLengths) {
    const std::vector<uint8_t> data = {'a', 'b', 'c'};

    EXPECT_EQ(static_cast<size_t>(32), Stribog::hash256(data).size());
    EXPECT_EQ(static_cast<size_t>(64), Stribog::hash512(data).size());
}

TEST(StribogTest, InstanceCalculationMatchesStaticHash) {
    const std::vector<uint8_t> data = {'m', 'e', 's', 's', 'a', 'g', 'e'};
    Stribog stribog256(HashLength::b256);
    Stribog stribog512(HashLength::b512);

    EXPECT_EQ(Stribog::hash256(data), stribog256.calculate(data));
    EXPECT_EQ(Stribog::hash512(data), stribog512.calculate(data));
}

// ================== GOST Signature ==================
TEST(GostSignatureTest, RejectsOutOfRangeSignatureValues) {
    SignatureParams params = SignatureParamsSet::CryptoPro_A;
    const ECPoint publicKey(params.p, params.a, params.b, params.x, params.y);
    StaticPublicKeyProvider publicKeyProvider(publicKey);
    SignatureProcessor processor;

    const std::vector<uint8_t> message = {'t', 'e', 's', 't'};
    const std::vector<uint8_t> zeroSignature(64, 0);

    EXPECT_FALSE(processor.verify(message, zeroSignature, &publicKeyProvider));
}

TEST(GostSignatureTest, ShortSignatureThrows) {
    SignatureParams params = SignatureParamsSet::CryptoPro_A;
    const ECPoint publicKey(params.p, params.a, params.b, params.x, params.y);
    StaticPublicKeyProvider publicKeyProvider(publicKey);
    SignatureProcessor processor;

    bool thrown = false;
    try {
        processor.verify({'x'}, {1, 2, 3}, &publicKeyProvider);
    } catch (const std::runtime_error&) {
        thrown = true;
    }

    EXPECT_TRUE(thrown);
}

TEST(GostSignatureTest, SmallCurvePointOperations) {
    SignatureParams params = SignatureParamsSet::Test;
    const ECPoint point(params.p, params.a, params.b, params.x, params.y);

    EXPECT_TRUE(ECPoint::isZero(point * BigInteger(0)));
    EXPECT_EQ(point, point * BigInteger(1));
    EXPECT_TRUE((point + ECPoint(params.p, params.a, params.b, 0, 0)) == point);
    EXPECT_EQ(BigInteger(10).toString(), ModMath::mul_inverse(BigInteger(3), BigInteger(29)).toString());
    EXPECT_EQ(BigInteger(28).toString(), ModMath::mod(BigInteger(-1), BigInteger(29)).toString());
}

// ================== CAdESBuilder ==================
TEST(CAdESBuilderTest, BuildCAdesBes) {
    CAdESBuilder::SignatureData data;
    data.signedData = {'d', 'a', 't', 'a'};
    data.signature = std::vector<uint8_t>(64, 0x11);
    data.certificate = {0x30, 0x01, 0x00};
    data.signingTime = "20260526120000Z";
    data.digestAlgorithm = "1.2.643.7.1.1.2.2";
    data.signatureAlgorithm = "1.2.643.7.1.1.1.1";

    CAdESBuilder builder;
    const std::vector<uint8_t> cades = builder.buildCAdES_BES(data);

    EXPECT_FALSE(cades.empty());
    EXPECT_EQ(static_cast<uint8_t>(0x30), cades.front());
}

TEST(CAdESBuilderTest, BuildCAdesBesWithLongPayload) {
    CAdESBuilder::SignatureData data;
    data.signedData = std::vector<uint8_t>(300, 0xAB);
    data.signature = std::vector<uint8_t>(128, 0xCD);
    data.certificate = std::vector<uint8_t>(140, 0xEF);
    data.signingTime = "20260526120000Z";
    data.digestAlgorithm = "1.2.840.113549.2.5";
    data.signatureAlgorithm = "1.2.840.113549.1.1.1";

    CAdESBuilder builder;
    const std::vector<uint8_t> cades = builder.buildCAdES_BES(data);

    EXPECT_TRUE(cades.size() > data.signedData.size());
    EXPECT_EQ(static_cast<uint8_t>(0x30), cades.front());
}
