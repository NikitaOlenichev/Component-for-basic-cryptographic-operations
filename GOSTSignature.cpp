//
// Генерирует ключевую пару, подписывает сообщение и проверяет подпись, используя алгоритм по ГОСТ Р 34.10-2012.
//

#include "GOSTSignature.h"
#include "Streebog.h"
#include <stdexcept>

EllipticPoint GOSTSignature::generator;
BigInteger GOSTSignature::p, GOSTSignature::a, GOSTSignature::b, GOSTSignature::q;
bool GOSTSignature::curveInitialized = false;

GOSTSignature::GOSTSignature() {
    if (!curveInitialized) {
        initializeCurve();
    }
}

void GOSTSignature::initializeCurve(int bits) {
    if (bits == 256) {
        // заменить на реальные параметры paramSetA для 256 бит
        p = BigInteger::fromBytes({ });
        a = BigInteger::fromBytes({ });
        b = BigInteger::fromBytes({ });
        q = BigInteger::fromBytes({ });
        generator = EllipticPoint(
            BigInteger::fromBytes({ }),
            BigInteger::fromBytes({ })
        );
    } else if (bits == 512) {
        // заменить на реальные параметры paramSetA для 512 бит
    } else {
        throw std::runtime_error("Invalid curve bit length. Must be 256 or 512.");
    }

    EllipticPoint::setCurveParams(p, a, b, q, generator);
    curveInitialized = true;
}

void GOSTSignature::generateKeyPair() {
    do {
        privateKey = BigInteger::random(q - 1);
    } while (privateKey <= 1);

    publicKey = generator * privateKey;
}

void GOSTSignature::setPrivateKey(const BigInteger& key) {
    privateKey = key;
    publicKey = generator * privateKey;
}

void GOSTSignature::setPublicKey(const EllipticPoint& key) {
    publicKey = key;
}

GOSTSignature::Signature GOSTSignature::sign(const std::vector<uint8_t> &message) const {
    if (privateKey.isZero()) {
        throw std::runtime_error("Private key is not set.");
    }

    std::vector<uint8_t> hash = Streebog::hash(message, 256);

    BigInteger alpha = BigInteger::fromBytes(hash, true);

    BigInteger k, e;
    EllipticPoint C;
    BigInteger r, s;

    do {
        do {
            k = BigInteger::random(q - 2) + 1;
            C = generator * k;
            r = C.getX() % q;
        } while (r.isZero());

        e = alpha % q;
        if (e.isZero()) e = 1;

        s = (r * privateKey + k * e) % q;
    } while (s.isZero());

    Signature signature;
    signature.r = r;
    signature.s = s;
    return signature;
}

bool GOSTSignature::verify(const std::vector<uint8_t> &message, const Signature &signature) const {
    if (signature.r.isZero() || signature.r >= q) return false;
    if (signature.s.isZero() || signature.s >= q) return false;

    std::vector<uint8_t> hash = Streebog::hash(message, 256);
    BigInteger alpha = BigInteger::fromBytes(hash, true);
    BigInteger e = alpha % q;
    if (e.isZero()) e = 1;

    BigInteger v = e.modInverse(q);

    BigInteger z1 = (signature.s * v) % q;
    BigInteger z2 = (q - (signature.r * v) % q) % q;

    EllipticPoint C = (generator * z1) + (publicKey * z2);

    return (C.getX() % q) == signature.r;
}

std::vector<uint8_t> GOSTSignature::Signature::toBytes() const {
    std::vector<uint8_t> rBytes = r.toBytes(32);
    std::vector<uint8_t> sBytes = s.toBytes(32);
    std::vector<uint8_t> result;
    result.insert(result.end(), rBytes.begin(), rBytes.end());
    result.insert(result.end(), sBytes.begin(), sBytes.end());
    return result;
}

GOSTSignature::Signature GOSTSignature::Signature::fromBytes(const std::vector<uint8_t> &bytes) {
    if (bytes.size() < 64)
        throw std::runtime_error("Invalid signature size");
    std::vector<uint8_t> rBytes(bytes.begin(), bytes.begin() + 32);
    std::vector<uint8_t> sBytes(bytes.begin() + 32, bytes.end());
    Signature sig;
    sig.r = BigInteger::fromBytes(rBytes, true);
    sig.s = BigInteger::fromBytes(sBytes, true);
    return sig;
}
