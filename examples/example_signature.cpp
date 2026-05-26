#include <CryptoComponent.h>
#include "console_utf8.h"
#include <iostream>
#include <vector>

int main() {
    setupConsoleUtf8();

    std::string privateKeyHex = "7A929ADE789BB3A7D6307356468277FADF461BBAB1C5C170B191710CC96441EC";
    std::string publicKeyXHex  = "1F16EF43F2B2EB6F067CABB881236041EB2F1839AA83ED9F87B5CB7FA7F77F4E";
    std::string publicKeyYHex  = "4D9F3640796615F047D444D4227F0EDC4DA37DAA23D323C4DE92EAD6171B70F5";
    std::vector<uint8_t> msg = {0x54, 0x65, 0x73, 0x74}; // "Test"

    // Подпись
    std::vector<uint8_t> signature = cryptocomponent::CryptoComponent::gostSign(msg, privateKeyHex);
    std::cout << "Подпись (hex): ";
    for (auto b : signature)
        std::cout << std::hex << (int)b;
    std::cout << std::dec << "\n";

    // Проверка
    bool ok = cryptocomponent::CryptoComponent::gostVerify(msg, signature, publicKeyXHex, publicKeyYHex);
    std::cout << "Проверка подписи: " << (ok ? "ВЕРНА" : "НЕВЕРНА") << "\n";

    // CAdES-обёртка
    std::vector<uint8_t> cades = cryptocomponent::CryptoComponent::cadesWrap(msg, signature);
    std::cout << "CAdES-BES размер: " << cades.size() << " байт\n";

    return 0;
}
