//
// Генерирует ключевую пару, подписывает сообщение и проверяет подпись, используя алгоритм по ГОСТ Р 34.10-2012.
//

#ifndef GOSTSIGNATURE_H
#define GOSTSIGNATURE_H

#include "BigInteger.h"
#include "EllipticPoint.h"
#include <vector>

class GOSTSignature {
public:
    GOSTSignature();

    void generateKeyPair();
    void setPrivateKey(const BigInteger& privateKey);
    void setPublicKey(const EllipticPoint& publicKey);

    struct Signature {
        BigInteger r;
        BigInteger s;

        std::vector<uint8_t> toBytes() const;
        static Signature fromBytes(const std::vector<uint8_t>& bytes);
    };

    Signature sign(const std::vector<uint8_t>& message) const;
    bool verify(const std::vector<uint8_t>& message, const Signature& signature) const;

    const EllipticPoint& getPublicKey() const {
        return publicKey;
    }
    const BigInteger& getPrivateKey() const {
        return privateKey;
    }

    static void initializeCurve(int bits = 256);

private:
    BigInteger privateKey;
    EllipticPoint publicKey;

    static EllipticPoint generator;
    static BigInteger p, a, b, q;
    static bool curveInitialized;
};

#endif //GOSTSIGNATURE_H
