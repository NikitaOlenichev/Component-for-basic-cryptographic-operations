// Реализация простого шифра Цезаря для английского языка
#include <iostream>
#include <string>
using namespace std;

string caesarCypher(const string& text, int shift) {
    string result = "";
    for (char c : text) {
        if (c >= 'a' && c <= 'z') {
            int new_pos = (c - 'a' + shift) % 26;
            if (new_pos < 0) new_pos += 26;
            result += char(new_pos + 'a');
        } else if (c >= 'A' && c <= 'Z') {
            int new_pos = (c - 'A' + shift) % 26;
            if (new_pos < 0) new_pos += 26;
            result += char(new_pos + 'A');
        } else {
            result += c;
        }
    }
    return result;
}
