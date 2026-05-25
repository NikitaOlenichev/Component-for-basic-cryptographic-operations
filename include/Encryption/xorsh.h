// xorsh.h
#ifndef XORSH_H
#define XORSH_H

#include <string>

std::string base64_encode(const std::string& data);
std::string base64_decode(const std::string& encoded);
std::string xorCrypt(const std::string& data, const std::string& key);
void encryptFile(const std::string& filename, const std::string& key);
void decryptFile(const std::string& filename, const std::string& key);

#endif // XORSH_H
