#pragma once
#include <string>

std::string base64_encode(unsigned char const* src, unsigned int len);
std::string base64_decode(const std::string_view s);
void base64_decode(const std::string_view _encoded_string, unsigned char* _dest, unsigned int _destLen);