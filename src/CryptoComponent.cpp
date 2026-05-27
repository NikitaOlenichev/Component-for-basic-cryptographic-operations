#include "CryptoComponent.h"

#include "Decryption/des.h"
#include "Encryption/DEScypher.h"
#include "Encryption/rc4_encrypt.h"
#include "Encryption/shifratbasha.h"
#include "Encryption/xorsh.h"
#ifdef USE_OPENSSL
#include "Hash/HashCalculate.h"
#endif
#include "Signatures/Stribog.h"
#include "Signatures/GOSTSignature.h"
#include "Signatures/CAdESBuilder.h"
#include "Signatures/BigInteger.h"

#include <iomanip>
#include <sstream>
#include <vector>
#include <string>

namespace cryptocomponent {

// Вспомогательные функции
namespace {
    std::string bytesToHex(const std::vector<uint8_t>& bytes) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (uint8_t b : bytes)
            ss << std::setw(2) << static_cast<int>(b);
        return ss.str();
    }

    std::vector<uint8_t> hexToBytes(const std::string& hex) {
        std::vector<uint8_t> bytes;
        for (size_t i = 0; i < hex.length(); i += 2) {
            std::string byteStr = hex.substr(i, 2);
            uint8_t byte = static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16));
            bytes.push_back(byte);
        }
        return bytes;
    }

    std::vector<uint8_t> xorBytes(const std::vector<uint8_t>& data, const std::vector<uint8_t>& key) {
        std::vector<uint8_t> result(data.size());
        for (size_t i = 0; i < data.size(); ++i)
            result[i] = data[i] ^ key[i % key.size()];
        return result;
    }
}

// Хеширование
#ifdef USE_OPENSSL
std::string CryptoComponent::md5(const std::string& data) {
    return HashCalculate::hashString(data, "MD5");
}

std::string CryptoComponent::md5File(const std::string& path) {
    return HashCalculate::hashFile(path, "MD5");
}

std::string CryptoComponent::sha1(const std::string& data) {
    return HashCalculate::hashString(data, "SHA1");
}

std::string CryptoComponent::sha256(const std::string& data) {
    return HashCalculate::hashString(data, "SHA256");
}

std::string CryptoComponent::sha512(const std::string& data) {
    return HashCalculate::hashString(data, "SHA512");
}

std::string CryptoComponent::hmacMD5(const std::string& data, const std::string& key) {
    return HashCalculate::hmacString(data, key, "MD5");
}

std::string CryptoComponent::hmacSHA1(const std::string& data, const std::string& key) {
    return HashCalculate::hmacString(data, key, "SHA1");
}

std::string CryptoComponent::hmacSHA256(const std::string& data, const std::string& key) {
    return HashCalculate::hmacString(data, key, "SHA256");
}

std::string CryptoComponent::hmacSHA512(const std::string& data, const std::string& key) {
    return HashCalculate::hmacString(data, key, "SHA512");
}

std::string CryptoComponent::hash(const std::string& data, const std::string& algorithm) {
    return HashCalculate::hashString(data, algorithm);
}

std::string CryptoComponent::hashFile(const std::string& path, const std::string& algorithm) {
    return HashCalculate::hashFile(path, algorithm);
}
#endif

std::vector<uint8_t> CryptoComponent::stribog256(const std::vector<uint8_t>& data) {
    return Stribog::hash256(data);
}

std::vector<uint8_t> CryptoComponent::stribog512(const std::vector<uint8_t>& data) {
    return Stribog::hash512(data);
}

std::string CryptoComponent::stribog256Hex(const std::string& data) {
    std::vector<uint8_t> bytes(data.begin(), data.end());
    auto hash = stribog256(bytes);
    return bytesToHex(hash);
}

std::string CryptoComponent::stribog512Hex(const std::string& data) {
    std::vector<uint8_t> bytes(data.begin(), data.end());
    auto hash = stribog512(bytes);
    return bytesToHex(hash);
}

// Симметричное шифрование
std::vector<uint8_t> CryptoComponent::rc4(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data) {
    RC4_Encryption rc4(key);
    return rc4.encrypt(data);
}

std::string CryptoComponent::rc4String(const std::string& key, const std::string& data) {
    std::vector<uint8_t> keyBytes(key.begin(), key.end());
    std::vector<uint8_t> dataBytes(data.begin(), data.end());
    auto enc = rc4(keyBytes, dataBytes);
    return bytesToHex(enc);
}

uint64_t CryptoComponent::desEncryptBlock(uint64_t block, uint64_t key) {
    return DEScypher::encrypt(block, key);
}

uint64_t CryptoComponent::desDecryptBlock(uint64_t block, uint64_t key) {
    return DES::decrypt(block, key);
}

std::string CryptoComponent::atbash(const std::string& text) {
    shifratbasha cipher;
    return cipher.encrypt(text);
}

std::string CryptoComponent::caesar(const std::string& text, int shift) {
    std::string result = text;
    for (char& c : result) {
        if (c >= 'a' && c <= 'z')
            c = static_cast<char>((c - 'a' + shift % 26 + 26) % 26 + 'a');
        else if (c >= 'A' && c <= 'Z')
            c = static_cast<char>((c - 'A' + shift % 26 + 26) % 26 + 'A');
    }
    return result;
}

std::vector<uint8_t> CryptoComponent::xorCipher(const std::vector<uint8_t>& data, const std::vector<uint8_t>& key) {
    return xorBytes(data, key);
}

std::string CryptoComponent::xorCipherString(const std::string& data, const std::string& key) {
    std::vector<uint8_t> dataBytes(data.begin(), data.end());
    std::vector<uint8_t> keyBytes(key.begin(), key.end());
    auto enc = xorBytes(dataBytes, keyBytes);
    return bytesToHex(enc);
}

// Асимметричное шифрование
uint64_t CryptoComponent::rsaModPow(uint64_t a, uint64_t b, uint64_t m) {
    uint64_t result = 1;
    a %= m;
    while (b > 0) {
        if (b & 1) result = (result * a) % m;
        a = (a * a) % m;
        b >>= 1;
    }
    return result;
}

uint64_t CryptoComponent::rsaEncrypt(uint64_t message, uint64_t e, uint64_t n) {
    return rsaModPow(message, e, n);
}

uint64_t CryptoComponent::rsaDecrypt(uint64_t cipher, uint64_t d, uint64_t n) {
    return rsaModPow(cipher, d, n);
}

std::string CryptoComponent::rsaEncryptString(const std::string& text, uint64_t e, uint64_t n) {
    std::stringstream ss;
    for (char c : text) {
        uint64_t enc = rsaEncrypt(static_cast<uint8_t>(c), e, n);
        ss << enc << " ";
    }
    return ss.str();
}

std::string CryptoComponent::rsaDecryptString(const std::string& ciphertext, uint64_t d, uint64_t n) {
    std::stringstream ss(ciphertext);
    std::string result;
    uint64_t num;
    while (ss >> num) {
        uint64_t dec = rsaDecrypt(num, d, n);
        result += static_cast<char>(dec);
    }
    return result;
}

// ГОСТ Р 34.10-2012
std::vector<uint8_t> CryptoComponent::gostSign(const std::vector<uint8_t>& msg, const std::string& privateKeyHex) {
    BigInteger privateKey = BigInteger::fromHex(privateKeyHex);
    SignatureProcessor processor;
    return processor.generate(msg, privateKey);
}

bool CryptoComponent::gostVerify(const std::vector<uint8_t>& msg, const std::vector<uint8_t>& signature,
                           const std::string& publicKeyXHex, const std::string& publicKeyYHex) {
    SignatureParams params = SignatureParamsSet::CryptoPro_A;
    ECPoint publicKey(params.p, params.a, params.b,
                      BigInteger::fromHex(publicKeyXHex),
                      BigInteger::fromHex(publicKeyYHex));

    class SimplePublicKeyProvider : public SignaturePublicKeyProvider {
        ECPoint key;
    public:
        SimplePublicKeyProvider(const ECPoint& k) : key(k) {}
        ECPoint get_key() override { return key; }
    };

    SignatureProcessor processor;
    SimplePublicKeyProvider provider(publicKey);
    return processor.verify(msg, signature, &provider);
}

std::vector<uint8_t> CryptoComponent::cadesWrap(const std::vector<uint8_t>& signedData, const std::vector<uint8_t>& signature) {
    CAdESBuilder builder;
    CAdESBuilder::SignatureData sigData;
    sigData.signedData = signedData;
    sigData.signature = signature;
    sigData.certificate = {};
    sigData.signingTime = "20250523120000Z";
    sigData.digestAlgorithm = "1.2.643.7.1.1.2.2";
    sigData.signatureAlgorithm = "1.2.643.7.1.1.1.1";
    return builder.buildCAdES_BES(sigData);
}

} // namespace cryptocomponent