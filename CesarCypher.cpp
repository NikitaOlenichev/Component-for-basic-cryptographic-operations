// Реализация простого шифра Цезаря для английского языка
#include <iostream>
#include <string>
using namespace std;

void caesarCypher() {
    string text;
    int shift;
    int choice;
    
    cout << "Выберите действие:\n";
    cout << "1 - Зашифровать текст\n";
    cout << "2 - Расшифровать текст\n";
    cout << "Ваш выбор: ";
    cin >> choice;
    cin.ignore(); // Очищаем буфер после ввода числа
    
    cout << "Введите текст: ";
    getline(cin, text);
    
    cout << "Введите сдвиг: ";
    cin >> shift;
    
    // Если выбрали расшифровку, меняем знак сдвига
    if (choice == 2) {
        shift = -shift;
        cout << "\n--- Режим расшифровки ---\n";
    } else if (choice == 1) {
        cout << "\n--- Режим шифрования ---\n";
    } else {
        cout << "\nОшибка: неверный выбор! Завершение программы.\n";
        return;
    }
    
    string result = "";
    
    // Проходим по всем символам
    for (int i = 0; i < text.length(); i++) {
        char c = text[i];
        
        // Проверяем, английская буква ли это
        if (c >= 'a' && c <= 'z') {
            // Маленькие английские буквы
            int new_pos = (c - 'a' + shift) % 26;
            if (new_pos < 0) new_pos += 26; // Для отрицательных значений
            result += char(new_pos + 'a');
        }
        else if (c >= 'A' && c <= 'Z') {
            // Большие английские буквы
            int new_pos = (c - 'A' + shift) % 26;
            if (new_pos < 0) new_pos += 26; // Для отрицательных значений
            result += char(new_pos + 'A');
        }
        else {
            // Остальные символы (пробелы, знаки препинания, цифры) оставляем как есть
            result += c;
        }
    }
    
    cout << "Результат: " << result << endl;
}

int main() {
    
    char again;
    do {
        caesarCypher();
        
        cout << "\nХотите повторить? (y/n): ";
        cin >> again;
        cin.ignore(); // Очищаем буфер
        cout << endl;
        
    } while (again == 'y' || again == 'Y');
    
    cout << "ПРограмма завершена\n";
    
    return 0;
}
