//
// Формат CAdES (CMS Advanced Electronic Signatures) делает подпись юридически значимой,
// добавляя к ней сертификаты и другую служебную информацию.
//

#include <vector>
#include <string>
#include <cstdint>

class CAdESBuilder {
public:
    struct SignatureData {
        std::vector<uint8_t> signedData;
        std::vector<uint8_t> signature;
        std::vector<uint8_t> certificate;
        std::string signingTime;
        std::string digestAlgorithm;
        std::string signatureAlgorithm;
    };

    std::vector<uint8_t> buildCAdES_BES(const SignatureData& data);

private:
    std::vector<uint8_t> encodeInteger(int value);
    std::vector<uint8_t> encodeOctetString(const std::vector<uint8_t>& data);
    std::vector<uint8_t> encodeObjectIdentifier(const std::string& oid);
    std::vector<uint8_t> encodeSequence(const std::vector<std::vector<uint8_t>>& components);
    std::vector<uint8_t> encodeSet(const std::vector<std::vector<uint8_t>>& components);
    std::vector<uint8_t> encodeUTCTime(const std::string& time);

    std::vector<uint8_t> encodeLength(size_t length);
};

#ifndef CADESBUILDER_H
#define CADESBUILDER_H

#endif //CADESBUILDER_H
