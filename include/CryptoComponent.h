#ifndef CRYPTOCOMPONENT_H
#define CRYPTOCOMPONENT_H

#include <string>
#include <vector>
#include <cstdint>

#include "export.h"

namespace cryptocomponent {

class CRYPTOCOMPONENT_API CryptoComponent {
public:
    // Хеширование
    // MD5, SHA
    static CRYPTOCOMPONENT_API std::string md5(const std::string& data);
    static CRYPTOCOMPONENT_API std::string md5File(const std::string& path);
    static CRYPTOCOMPONENT_API std::string sha1(const std::string& data);
    static CRYPTOCOMPONENT_API std::string sha256(const std::string& data);
    static CRYPTOCOMPONENT_API std::string sha512(const std::string& data);

    //ГОСТ Стрибог
    static CRYPTOCOMPONENT_API std::vector<uint8_t> stribog256(const std::vector<uint8_t>& data);
    static CRYPTOCOMPONENT_API std::vector<uint8_t> stribog512(const std::vector<uint8_t>& data);
    static CRYPTOCOMPONENT_API std::string stribog256Hex(const std::string& data);
    static CRYPTOCOMPONENT_API std::string stribog512Hex(const std::string& data);

    // HMAC
    static CRYPTOCOMPONENT_API std::string hmacMD5(const std::string& data, const std::string& key);
    static CRYPTOCOMPONENT_API std::string hmacSHA1(const std::string& data, const std::string& key);
    static CRYPTOCOMPONENT_API std::string hmacSHA256(const std::string& data, const std::string& key);
    static CRYPTOCOMPONENT_API std::string hmacSHA512(const std::string& data, const std::string& key);

    // Универсальный метод
    static CRYPTOCOMPONENT_API std::string hash(const std::string& data, const std::string& algorithm);
    static CRYPTOCOMPONENT_API std::string hashFile(const std::string& path, const std::string& algorithm);

    // Симметричное шифрование
    // RC4
    static CRYPTOCOMPONENT_API std::vector<uint8_t> rc4(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data);
    static CRYPTOCOMPONENT_API std::string rc4String(const std::string& key, const std::string& data);

    // DES
    static CRYPTOCOMPONENT_API uint64_t desEncryptBlock(uint64_t block, uint64_t key);
    static CRYPTOCOMPONENT_API uint64_t desDecryptBlock(uint64_t block, uint64_t key);

    // Atbash
    static CRYPTOCOMPONENT_API std::string atbash(const std::string& text);

    // Шифр Цезаря
    static CRYPTOCOMPONENT_API std::string caesar(const std::string& text, int shift);

    // XOR с ключом
    static CRYPTOCOMPONENT_API std::vector<uint8_t> xorCipher(const std::vector<uint8_t>& data, const std::vector<uint8_t>& key);
    static CRYPTOCOMPONENT_API std::string xorCipherString(const std::string& data, const std::string& key);

    // Асимметричное шифрование (RSA)
    static CRYPTOCOMPONENT_API uint64_t rsaModPow(uint64_t a, uint64_t b, uint64_t m);
    static CRYPTOCOMPONENT_API uint64_t rsaEncrypt(uint64_t message, uint64_t e, uint64_t n);
    static CRYPTOCOMPONENT_API uint64_t rsaDecrypt(uint64_t cipher, uint64_t d, uint64_t n);
    static CRYPTOCOMPONENT_API std::string rsaEncryptString(const std::string& text, uint64_t e, uint64_t n);
    static CRYPTOCOMPONENT_API std::string rsaDecryptString(const std::string& ciphertext, uint64_t d, uint64_t n);

    // ГОСТ Р 34.10-2012 (цифровая подпись)
    // Генерация подписи
    static CRYPTOCOMPONENT_API std::vector<uint8_t> gostSign(const std::vector<uint8_t>& msg, const std::string& privateKeyHex);

    // Проверка подписи
    static CRYPTOCOMPONENT_API bool gostVerify(const std::vector<uint8_t>& msg, const std::vector<uint8_t>& signature,
                           const std::string& publicKeyXHex, const std::string& publicKeyYHex);

    // Упаковка подписи в CAdES-BES
    static CRYPTOCOMPONENT_API std::vector<uint8_t> cadesWrap(const std::vector<uint8_t>& signedData, const std::vector<uint8_t>& signature);
};

} // namespace cryptocomponent

#endif // CRYPTOCOMPONENT_H