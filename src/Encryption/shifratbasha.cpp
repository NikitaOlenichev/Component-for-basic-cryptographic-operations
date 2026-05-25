#include <iostream>
#include "Encryption/shifratbasha.h"

char atbashChar(char c) {
    if (c >= 'A' && c <= 'Z') {
        return 'Z' - (c - 'A');
    }
    if (c >= 'a' && c <= 'z') {
        return 'z' - (c - 'a');
    }
    return c;
}

std::string shifratbasha::encrypt(const std::string& text) {
    std::string result = text;

    for (int i = 0; i < text.length(); i++) {
        result[i] = atbashChar(text[i]);
    }

    return result;
}

std::string shifratbasha::decrypt(const std::string& text) {
    return encrypt(text);
}
