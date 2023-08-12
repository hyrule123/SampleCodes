#include "json-cpp\json.h"
#include <fstream>
#include <string>

#include "base64.h"
#include <iostream>

int main()
{
    //const std::string s = "ADP GmbH\nAnalyse Design & Programmierung\nGesellschaft mit beschränkter Haftung";

    int s = 50192548;

    std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(&s), sizeof(s));
    //std::string decoded = base64_decode(encoded);
    int decoded = *(int*)base64_decode(encoded).c_str();

    std::cout << "encoded: " << encoded << std::endl;
    std::cout << "decoded: " << decoded << std::endl;


    return 0;
}