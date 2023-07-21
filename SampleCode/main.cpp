#include <iostream>
#include <typeinfo>
#include <typeindex>

int main() {
    std::cout << "Size of std::type_info: " << sizeof(std::type_info) << " bytes" << std::endl;
    std::cout << "Size of std::type_index: " << sizeof(std::type_index) << " bytes" << std::endl;
    return 0;
}