#include <CryptoComponent.h>
#include <iostream>
#include <vector>

int main() {
    std::string privateKeyHex = "7A929ADE789BB3A7D6307356468277FADF461BBAB1C5C170B191710CC96441EC";
    std::string publicKeyXHex  = "7F2B49E9D5286931E9762F6499B74316F169762F6499B74316F169762F6499B7";
    std::string publicKeyYHex  = "48169762F6499B74316F169762F6499B74316F169762F6499B74316F169762F6";
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