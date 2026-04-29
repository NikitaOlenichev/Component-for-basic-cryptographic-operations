//
// Формат CAdES (CMS Advanced Electronic Signatures) делает подпись юридически значимой,
// добавляя к ней сертификаты и другую служебную информацию.
//

#include "CAdESBuilder.h"
#include <algorithm>
#include <sstream>
#include <ctime>
#include <stdexcept>

std::vector<uint8_t> CAdESBuilder::buildCAdES_BES(const SignatureData &data) {
    std::vector<std::vector<uint8_t>> signerInfoComponents;

    signerInfoComponents.push_back(encodeInteger(3));

    std::vector<std::vector<uint8_t>> digestAlgorithmsSet;
    digestAlgorithmsSet.push_back(encodeSequence({
        encodeObjectIdentifier(data.digestAlgorithm),
        encodeOctetString({})
    }));
    signerInfoComponents.push_back(encodeSet(digestAlgorithmsSet));

    std::vector<std::vector<uint8_t>> contentInfoComponents;
    contentInfoComponents.push_back(encodeObjectIdentifier("1.2.840.113549.1.7.1"));
    contentInfoComponents.push_back(encodeOctetString(data.signedData));
    signerInfoComponents.push_back(encodeSequence(contentInfoComponents));

    std::vector<std::vector<uint8_t>> certificatesSet;
    certificatesSet.push_back(encodeOctetString(data.certificate));
    signerInfoComponents.push_back(encodeSet(certificatesSet));

    std::vector<std::vector<uint8_t>> signerInfoComponentsInner;
    signerInfoComponentsInner.push_back(encodeInteger(1));
    signerInfoComponentsInner.push_back(encodeOctetString(data.certificate));
    signerInfoComponentsInner.push_back(encodeSequence({
        encodeObjectIdentifier(data.signatureAlgorithm),
        encodeOctetString({})
    }));
    signerInfoComponentsInner.push_back(encodeOctetString(data.signature));

    std::vector<std::vector<uint8_t>> signedAttrs;
    signedAttrs.push_back(encodeSequence({
        encodeObjectIdentifier("1.2.840.113549.1.9.5"),
        encodeSet({encodeUTCTime(data.signingTime)})
    }));
    signerInfoComponentsInner.push_back(encodeSet(signedAttrs));

    signerInfoComponents.push_back(encodeSet({encodeSequence(signerInfoComponentsInner)}));

    return encodeSequence(signerInfoComponents);
}

std::vector<uint8_t> CAdESBuilder::encodeInteger(int value) {
    std::vector<uint8_t> result;
    result.push_back(0x02);
    if (value == 0) {
        result.push_back(1);
        result.push_back(0);
    } else {
        bool negative = (value < 0);
        uint32_t absVal = negative ? -value : value;
        std::vector<uint8_t> bytes;
        while (absVal > 0) {
            bytes.push_back(absVal & 0xFF);
            absVal >>= 8;
        }
        if (bytes.back() & 0x80)
            bytes.push_back(0x00);
        if (negative) {
            for (auto& b : bytes)
                b = ~b;
            for (size_t i = 0; i < bytes.size(); i++) {
                bytes[i] += 1;
                if (bytes[i] != 0)
                    break;
            }
        }
        std::reverse(bytes.begin(), bytes.end());
        auto lenEnc = encodeLength(bytes.size());
        result.insert(result.end(), lenEnc.begin(), lenEnc.end());
        result.insert(result.end(), bytes.begin(), bytes.end());
    }
    return result;
}

std::vector<uint8_t> CAdESBuilder::encodeOctetString(const std::vector<uint8_t> &data) {
    std::vector<uint8_t> result;
    result.push_back(0x04);
    std::vector<uint8_t> length = encodeLength(data.size());
    result.insert(result.end(), length.begin(), length.end());
    result.insert(result.end(), data.begin(), data.end());
    return result;
}

std::vector<uint8_t> CAdESBuilder::encodeObjectIdentifier(const std::string &oid) {
    std::vector<uint8_t> result;
    result.push_back(0x06);

    std::vector<int> components;
    std::stringstream ss(oid);
    std::string component;
    while (std::getline(ss, component, '.')) {
        components.push_back(std::stoi(component));
    }

    std::vector<uint8_t> encoded;
    encoded.push_back(components[0] * 40 + components[1]);
    for (size_t i = 2; i < components.size(); i++) {
        int value = components[i];
        if (value < 128) {
            encoded.push_back(value);
        } else {
            std::vector<uint8_t> tmp;
            while (value > 0) {
                tmp.push_back(value & 0x7F);
                value >>= 7;
            }
            std::reverse(tmp.begin(), tmp.end());
            for (size_t j = 0; j < tmp.size(); j++) {
                if (j != tmp.size() - 1)
                    tmp[j] |= 0x80;
                encoded.push_back(tmp[j]);
            }
        }
    }
    auto lenEnc = encodeLength(encoded.size());
    result.insert(result.end(), lenEnc.begin(), lenEnc.end());
    result.insert(result.end(), encoded.begin(), encoded.end());
    return result;
}

std::vector<uint8_t> CAdESBuilder::encodeSequence(const std::vector<std::vector<uint8_t> > &components) {
    std::vector<uint8_t> result;
    result.push_back(0x30);

    size_t totalLength = 0;
    for (const auto& component : components) {
        totalLength += component.size();
    }

    std::vector<uint8_t> length = encodeLength(totalLength);
    result.insert(result.end(), length.begin(), length.end());

    for (const auto& component : components) {
        result.insert(result.end(), component.begin(), component.end());
    }

    return result;
}

std::vector<uint8_t> CAdESBuilder::encodeSet(const std::vector<std::vector<uint8_t> > &components) {
    std::vector<uint8_t> result;
    result.push_back(0x31);

    size_t totalLength = 0;
    for (const auto& component : components) {
        totalLength += component.size();
    }

    std::vector<uint8_t> length = encodeLength(totalLength);
    result.insert(result.end(), length.begin(), length.end());

    for (const auto& component : components) {
        result.insert(result.end(), component.begin(), component.end());
    }

    return result;
}

std::vector<uint8_t> CAdESBuilder::encodeUTCTime(const std::string &time) {
    std::vector<uint8_t> result;
    result.push_back(0x17);

    std::time_t now = std::time(nullptr);
    std::tm* utc = std::gmtime(&now);
    char buf[16];
    std::strftime(buf, sizeof(buf), "%y%m%d%H%M%SZ", utc);
    std::string timeStr(buf);

    std::vector<uint8_t> length = encodeLength(timeStr.length());
    result.insert(result.end(), length.begin(), length.end());
    result.insert(result.end(), timeStr.begin(), timeStr.end());
    return result;
}

std::vector<uint8_t> CAdESBuilder::encodeLength(size_t length) {
    std::vector<uint8_t> result;
    if (length < 128) {
        result.push_back(static_cast<uint8_t>(length));
    } else if (length < 256) {
        result.push_back(0x81);
        result.push_back(static_cast<uint8_t>(length));
    } else if (length < 65536) {
        result.push_back(0x82);
        result.push_back(static_cast<uint8_t>((length >> 8) & 0xFF));
        result.push_back(static_cast<uint8_t>(length & 0xFF));
    } else {
        result.push_back(0x83);
        result.push_back(static_cast<uint8_t>((length >> 16) & 0xFF));
        result.push_back(static_cast<uint8_t>((length >> 8) & 0xFF));
        result.push_back(static_cast<uint8_t>(length & 0xFF));
    }

    return result;
}

