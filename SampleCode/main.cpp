#include <memory>
#include <type_traits>
#include <iostream>

#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <unordered_map>
#include <type_traits>

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

    // Base template for is_container (false by default)
    template <typename T, typename = void>
    struct is_container : std::false_type {};
    // Specializations for types that behave like containers
    template <typename T>
    struct is_container<T, std::void_t<
        typename T::value_type,
        decltype(std::declval<T>().begin()),
        decltype(std::declval<T>().end()),
        decltype(std::declval<T>().size())
        >> : std::true_type {};
    template <class T> inline constexpr bool is_container_v = is_container<T>::value;



    template <typename T, typename = void>
    struct is_pair_container : std::false_type {};
    template <typename T>
    struct is_pair_container<T, std::void_t<
        typename T::value_type,
        decltype(std::declval<typename T::value_type::first_type>()),
        decltype(std::declval<typename T::value_type::second_type>()),
        decltype(std::declval<T>().begin()),
        decltype(std::declval<T>().end()),
        decltype(std::declval<T>().size())
        >> : std::true_type{};
    template <class T> inline constexpr bool is_pair_container_v = is_pair_container<T>::value;
}


int main()
{
    std::vector<int> vecInt{ 1, 2, 3, 4, 5 };

    std::list<float> listFloat{ 1, 2, 3, 4, 5 };
    std::unordered_map<std::string, int> umapInt;

    constexpr bool test1 = std::is_container<std::vector<int>>::value;
    constexpr bool test2 = std::is_container<std::list<float>>::value;
    constexpr bool test3 = std::is_container<std::unordered_map<std::string, int>>::value;
    constexpr bool test4 = std::is_pair_container<std::unordered_map<std::string, int>>::value;
    constexpr bool test5 = std::is_pair_container<std::list<float>>::value;

    return 0;
}