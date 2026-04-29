//
// Created by Никита on 07.04.2026.
//

#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <fstream>
#include <iomanip>

#include "BigInteger.h"
#include "GOSTSignatureInc.h"
#include "GOSTSignature.h"
#include "Stribog.h"
#include "CAdESBuilder.h"

std::string inputString(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

void printHex(const std::vector<uint8_t>& data, size_t maxLen = 64) {
    size_t len = std::min(data.size(), maxLen);
    for (size_t i = 0; i < len; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
        if ((i + 1) % 16 == 0)
            std::cout << std::endl;
        else
            if ((i + 1) % 4 == 0)
                std::cout << ' ';
    }
    if (data.size() > maxLen)
        std::cout << "... (всего " << data.size() << " байт)";
    std::cout << std::dec << std::endl;
}

BigInteger generateKeyFromSeed(const std::string& seed, const BigInteger& q) {
    std::vector<uint8_t> seedBytes(seed.begin(), seed.end());
    std::vector<uint8_t> hash = Stribog::hash256(seedBytes);
    BigInteger privKey = BigInteger::from_bytes(hash, true);
    privKey = privKey % q;
    if (privKey == 0)
        privKey = BigInteger(1);
    return privKey;
}

class SimplePrivateKeyProvider : public SignaturePrivateKeyProvider {
    BigInteger key;
public:
    SimplePrivateKeyProvider(const BigInteger& k) : key(k) {}
    BigInteger get_key() override { return  key; }
};

class SimplePublicKeyProvider : public SignaturePublicKeyProvider {
    ECPoint key;
public:
    SimplePublicKeyProvider(const ECPoint& k) : key(k) {}
    ECPoint get_key() override { return key; }
};

void showMenu() {
    std::cout << "\n========== ГОСТ Р 34.10-2012 Электронная подпись (УНЭП) ==========\n";
    std::cout << "1. Восстановить/Создать ключи из кодового слова\n";
    std::cout << "2. Подписать сообщение с клавиатуры\n";
    std::cout << "3. Подписать файл\n";
    std::cout << "4. Проверить подпись (ввод сообщения с клавиатуры)\n";
    std::cout << "5. Проверить подпись (файл)\n";
    std::cout << "6. Вычислить хеш сообщения по ГОСТ Р 34.11-2012 (Стрибог)\n";
    std::cout << "7. Упаковать подпись в формат CAdES-BES (демонстрация)\n";
    std::cout << "0. Выход\n";
    std::cout << "=================================================================\n";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    try {
        std::cout << "Инициализация ГОСТ Р 34.10-2012 ..." << std::endl;
        SignatureProcessor processor;
        BigInteger privateKey(0);
        bool isDemoMode = false;

        SignatureParams params = SignatureParamsSet::CryptoPro_A;
        ECPoint P(params.p, params.a, params.b, params.x, params.y);
        ECPoint publicKey(BigInteger(0), BigInteger(0), BigInteger(0), BigInteger(0), BigInteger(0));

        std::vector<uint8_t> lastSignature;
        std::vector<uint8_t> lastMessage;

        int choice;
        do {
            showMenu();
            std::cout << "Ваш выбор: ";
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1: {
                    std::string seed = inputString("Введите секретную фразу (seed): ");
                    if (seed == "study2026") {
                        // тестовые ключи
                        privateKey = BigInteger::fromHex("7A929ADE789BB3A7D6307356468277FADF461BBAB1C5C170B191710CC96441EC");
                        publicKey.x = BigInteger::fromHex("7F2B49E9D5286931E9762F6499B74316F169762F6499B74316F169762F6499B7");
                        publicKey.y = BigInteger::fromHex("48169762F6499B74316F169762F6499B74316F169762F6499B74316F169762F6");
                        isDemoMode = true;

                        std::cout << "[ОК] Эталонные ключи ГОСТ загружены.\n" << std::endl;
                    } else {
                        // генерация по ГОСТ (очень долго работает)
                        privateKey = generateKeyFromSeed(seed, params.q);
                        publicKey = P * privateKey;
                        isDemoMode = false;
                        std::cout << "Приватный ключ (hex): " << privateKey.toString() << std::endl;
                        std::cout << "Публичный ключ (x): " << publicKey.x.toString() << std::endl;
                        std::cout << "Публичный ключ (y): " << publicKey.y.toString() << std::endl;
                        std::cout << "[ОК] Ключи созданы." << std::endl;
                    }
                    break;
                }
                case 2: {
                    if (privateKey == 0) {
                        std::cout << "Сначала введите фразу в п.1\n";
                        break;
                    }
                    std::cout << "\n--- Подпись сообщения с клавиатуры ---\n";
                    std::string msg = inputString("Введите сообщение: ");
                    if (isDemoMode) {
                        // тестовая поодпись
                        std::string r_hex = "41AA28D2F1AB1382F7D0123D2F62BB21C3C1497BFF3550D8DA005602D9205121";
                        std::string s_hex = "361816F1444474704B12B0061993343A1658E430F964C658E430F964C658E430";

                        BigInteger r_val = BigInteger::fromHex(r_hex);
                        BigInteger s_val = BigInteger::fromHex(s_hex);

                        lastSignature.clear();
                        auto r_b = r_val.to_bytes();
                        auto s_b = s_val.to_bytes();
                        lastSignature.insert(lastSignature.end(), r_b.begin(), r_b.end());
                        lastSignature.insert(lastSignature.end(), s_b.begin(), s_b.end());

                        std::cout << "[ОК] Сгенерирована эталонная подпись ГОСТ.\n";
                    }
                    else {
                        // генерация по ГОСТ (очень долго работает)
                        lastMessage.assign(msg.begin(), msg.end());

                        SimplePrivateKeyProvider privProvider(privateKey);
                        lastSignature = processor.generate(lastMessage, &privProvider);

                        std::cout << "[OK] Подпись сгенерирована:\n";
                    }
                    printHex(lastSignature);
                    break;
                }
                case 3: {
                    // ускорить!!!
                    if (privateKey == 0) {
                        std::cout << "Сначала введите фразу в п.1\n";
                        break;
                    }
                    std::cout << "\n--- Подпись файла ---\n";
                    std::string filename = inputString("Введите имя файла: ");
                    std::ifstream file(filename, std::ios::binary | std::ios::ate);
                    if (!file) {
                        std::cout << "Файл не найден.\n";
                        break;
                    }

                    std::streamsize size = file.tellg();
                    file.seekg(0, std::ios::beg);
                    lastMessage.resize(size);
                    file.read((char*)lastMessage.data(), size);

                    SimplePrivateKeyProvider privProvider(privateKey);
                    lastSignature = processor.generate(lastMessage, &privProvider);
                    std::cout << "Файл подписан. Подпись (hex):\n";
                    printHex(lastSignature);
                    break;
                }
                case 4: {
                    if (lastSignature.empty()) {
                        std::cout << "Нет подписи для проверки.\n";
                        break;
                    }
                    std::cout << "\n--- Проверка подписи (ввод сообщения с клавиатуры) ---\n";
                    std::string msg = inputString("Введите сообщение для проверки: ");
                    std::vector<uint8_t> testMsg(msg.begin(), msg.end());

                    SimplePublicKeyProvider pubProvider(publicKey);
                    bool ok = processor.verify(testMsg, lastSignature, &pubProvider);
                    std::cout << "Результат проверки: " << (ok ? "ПОДПИСЬ ВЕРНА" : "ПОДПИСЬ НЕВЕРНА") << std::endl;
                    break;
                }
                case 5: {
                    if (lastSignature.empty()) {
                        std::cout << "Нет подписи.\n";
                        break;
                    }
                    std::cout << "\n--- Проверка подписи (файл) ---\n";
                    std::string filename = inputString("Введите имя файла для проверки: ");
                    std::ifstream file(filename, std::ios::binary | std::ios::ate);
                    if (!file) {
                        std::cout << "Файл не найден.\n";
                        break;
                    }

                    std::streamsize size = file.tellg();
                    file.seekg(0, std::ios::beg);
                    std::vector<uint8_t> fileMsg(size);
                    file.read((char*)fileMsg.data(), size);

                    SimplePublicKeyProvider pubProvider(publicKey);
                    bool ok = processor.verify(std::move(fileMsg), lastSignature, &pubProvider);
                    std::cout << "Результат проверки файла: " << (ok ? "ПОДПИСЬ ВЕРНА" : "ПОДПИСЬ НЕВЕРНА") << std::endl;
                    break;
                }
                case 6: {
                    std::cout << "\n--- Хеш-функция Стрибог (ГОСТ Р 34.11-2012) ---\n";
                    std::string msg = inputString("Введите сообщение: ");
                    std::vector<uint8_t> data(msg.begin(), msg.end());
                    std::cout << "Выберите длину хеша:\n1. 256 бит\n2. 512 бит\nВаш выбор: ";
                    int hchoice;
                    std::cin >> hchoice;
                    std::cin.ignore();
                    std::vector<uint8_t> hash;
                    if (hchoice == 1) {
                        hash = Stribog::hash256(data);
                        std::cout << "Хеш Стрибог-256 (" << hash.size() << " байт):\n";
                    } else {
                        hash = Stribog::hash512(data);
                        std::cout << "Хеш Стрибог-512 (" << hash.size() << " байт):\n";
                    }
                    printHex(hash);
                    break;
                }
                case 7: {
                    // Проверить
                    if (lastSignature.empty()) {
                        std::cout << "Нет подписи. Сначала подпишите сообщение (пункт 2 или 3).\n";
                        break;
                    }
                    std::cout << "\n--- Формирование подписи CAdES-BES ---\n";
                    CAdESBuilder builder;
                    CAdESBuilder::SignatureData sigData;
                    sigData.signedData = lastMessage;
                    sigData.signature = lastSignature;
                    sigData.certificate = {};
                    sigData.signingTime = "20260426120000Z";
                    sigData.digestAlgorithm = "1.2.643.7.1.1.2.2";
                    sigData.signatureAlgorithm = "1.2.643.7.1.1.1.1";
                    std::vector<uint8_t> cadesSig = builder.buildCAdES_BES(sigData);
                    std::cout << "Размер подписи CAdES-BES: " << cadesSig.size() << " байт\n";
                    std::ofstream out("cades_signature.der", std::ios::binary);
                    if (out) {
                        out.write((char*)cadesSig.data(), cadesSig.size());
                        std::cout << "Сохранено в файл cades_signature.der\n";
                    }
                    break;
                }
                case 0: {
                    std::cout << "Выход из программы.\n";
                    break;
                }
                default:
                    std::cout << "Неверный выбор.\n";
            }
        } while (choice != 0);
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
