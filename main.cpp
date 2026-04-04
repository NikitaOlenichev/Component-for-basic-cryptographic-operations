#include "rsa.h"
#include <iostream>
#include <string>
#include <limits>
#include <windows.h>

using namespace std;
using namespace RSA;

int main() {
    // Настройка консоли для русского языка
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    cout << "=== RSA ШИФРОВАНИЕ (Visual Studio версия) ===\n\n";

    // Генерация ключей
    long long n, e, d;
    generateKeys(n, e, d);

    cout << "Открытый ключ (e, n): (" << e << ", " << n << ")\n";
    cout << "Закрытый ключ (d, n): (" << d << ", " << n << ")\n\n";

    // ===== РАБОТА С ЧИСЛАМИ =====
    cout << "--- Работа с числами ---\n";
    long long message;
    cout << "Введите число для шифрования (меньше " << n << "): ";
    cin >> message;

    if (message >= n) {
        cout << "Ошибка: сообщение должно быть меньше " << n << endl;
        return 1;
    }

    long long encrypted = encrypt(message, e, n);
    long long decrypted = decrypt(encrypted, d, n);

    cout << "Зашифрованное: " << encrypted << endl;
    cout << "Расшифрованное: " << decrypted << endl;

    // Очистка буфера ввода
    cin.ignore(1000, '\n');

    // ===== РАБОТА СО СТРОКАМИ =====
    cout << "\n--- Работа со строками ---\n";
    string text;
    cout << "Введите строку для шифрования: ";
    getline(cin, text);

    string encryptedText = encryptString(text, e, n);
    string decryptedText = decryptString(encryptedText, d, n);

    cout << "Исходная строка: " << text << endl;
    cout << "Зашифрованная: " << encryptedText << endl;
    cout << "Расшифрованная: " << decryptedText << endl;

    // Проверка
    if (text == decryptedText) {
        cout << "\n✓ Успех! Всё работает корректно!\n";
    }
    else {
        cout << "\n✗ Ошибка! Что-то пошло не так.\n";
    }

    cout << "\nНажмите Enter для выхода...";
    cin.ignore();
    cin.get();

    return 0;
}