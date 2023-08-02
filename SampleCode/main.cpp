#include <memory>
#include <type_traits>
#include <iostream>

namespace std
{
    template <class T> struct is_shared_ptr : std::false_type {};
    template <class T> struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};
    template <class T> inline constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;

    template <class T> struct is_unique_ptr : std::false_type {};
    template <class T> struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {};
    template <class T> inline constexpr bool is_unique_ptr_v = is_unique_ptr<T>::value;

    template <class T> struct is_smart_ptr : std::false_type {};
    template <class T> struct is_smart_ptr<std::shared_ptr<T>> : std::true_type {};
    template <class T> struct is_smart_ptr<std::unique_ptr<T>> : std::true_type {};
    template <class T> inline constexpr bool is_smart_ptr_v = is_smart_ptr<T>::value;
}


int main() 
{
    std::shared_ptr<int> pInt = std::make_shared<int>();
    int* ptr = new int;

    if (std::is_shared_ptr_v<std::shared_ptr<int>>)
    {
        std::cout << "This is shared pointer!!" << std::endl;
    }

    std::cout << "int* type is ";
    if (std::is_shared_ptr_v<int*>)
    {
        std::cout << "shared pointer !!" << std::endl;
    }
    else
    {
        std::cout << "not shared pointer !!" << std::endl;
    }
   
    std::cout << "std::unique_ptr<float*> is ";
    if(std::is_smart_ptr_v<std::unique_ptr<float*>>)
    {
        std::cout << "smart pointer !!" << std::endl;
    }
    else
    {
        std::cout << "not smart pointer !!" << std::endl;
    }


    delete ptr;
    return 0;
}