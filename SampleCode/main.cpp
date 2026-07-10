#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

// 통합된 FNV-1a 해시 함수 (constexpr이므로 둘 다 사용 가능)
static constexpr size_t hash_str_fnv1a(std::string_view s) {
    size_t hash = 14695981039346656037ULL;
    for (char c : s) {
        hash ^= static_cast<size_t>(c);
        hash *= 1099511628211ULL;
    }
    return hash;
}

struct PrehashedStr
{
    consteval PrehashedStr(const std::string_view s)
        : str(s), hash(hash_str_fnv1a(str)) {}

    PrehashedStr() = delete;
    ~PrehashedStr() = default;

    const std::string_view str;
    const size_t hash;
};

consteval PrehashedStr operator"" _hash(const char* str, size_t)
{
    return PrehashedStr(str);
}

struct PrehashedStrRuntime
{
    PrehashedStrRuntime() = delete;
    ~PrehashedStrRuntime() = default;

    explicit PrehashedStrRuntime(const std::string& s)
		: str(s), hash(hash_str_fnv1a(str)) {}

    const std::string str;
    const size_t hash;
};

struct StrHashEqual {
    using is_transparent = void;

    bool operator()(std::string_view lhs, std::string_view rhs) const { return lhs == rhs; }

    bool operator()(std::string_view lhs, const PrehashedStr& rhs) const { return lhs == rhs.str; }
    bool operator()(const PrehashedStr& lhs, std::string_view rhs) const { return lhs.str == rhs; }

    bool operator()(std::string_view lhs, const PrehashedStrRuntime& rhs) const { return lhs == rhs.str; }
    bool operator()(const PrehashedStrRuntime& lhs, std::string_view rhs) const { return lhs.str == rhs; }
};

struct StrHasher
{
	using is_transparent = void;
	size_t operator()(const std::string& s) const { return hash_str_fnv1a(s); }
	size_t operator()(std::string_view s) const { return hash_str_fnv1a(s); }
	size_t operator()(const PrehashedStr& pk) const { return pk.hash; }
	size_t operator()(const PrehashedStrRuntime& pk) const { return pk.hash; }
};

std::unordered_map<std::string, int, StrHasher, StrHashEqual> myMap;

// 4. 테스트 
int main() {
	myMap["PlayerHp"] = 100;

    auto it = myMap.find("PlayerHp"_hash);

    return 0;
}