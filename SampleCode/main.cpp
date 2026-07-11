#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <chrono>
#include <random>
#include <array>
#include <utility>

// =====================================================================
// 1. 기존 HashedStringView 및 투명 조회 유틸리티
// =====================================================================
static constexpr size_t hash_str_fnv1a(const std::string_view s) {
    size_t hash = 14695981039346656037ULL;
    for (char c : s) {
        hash ^= static_cast<size_t>(c);
        hash *= 1099511628211ULL;
    }
    return hash;
}

class HashedStringView {
public:
    constexpr HashedStringView(const std::string_view s) : hash(hash_str_fnv1a(s)), str_view(s) {}
    size_t GetHash() const { return hash; }
    std::string_view GetStringView() const { return str_view; }
private:
    friend class HashedString;
    explicit HashedStringView(const std::string_view s, size_t h) : hash(h), str_view(s) {}
    HashedStringView() = delete;
    size_t hash;
    std::string_view str_view;
};

consteval HashedStringView operator "" _hash(const char* s, size_t len) {
    return HashedStringView(std::string_view(s, len));
}

struct StringHasher {
    using is_transparent = void;
    size_t operator()(const std::string_view s) const noexcept { return hash_str_fnv1a(s); }
    size_t operator()(const std::string& s) const noexcept { return hash_str_fnv1a(s); }
    size_t operator()(const HashedStringView& s) const noexcept { return s.GetHash(); }
};

struct StringEqual {
    using is_transparent = void;
    bool operator()(const std::string_view lhs, const std::string_view rhs) const { return lhs == rhs; }
    bool operator()(const std::string_view lhs, const HashedStringView& rhs) const { return lhs == rhs.GetStringView(); }
    bool operator()(const HashedStringView& lhs, const std::string_view rhs) const { return lhs.GetStringView() == rhs; }
    bool operator()(const HashedStringView& lhs, const HashedStringView& rhs) const {
        return lhs.GetHash() == rhs.GetHash() && lhs.GetStringView() == rhs.GetStringView();
    }
};

template <typename T>
using StringHashTable = std::unordered_map<std::string, T, StringHasher, StringEqual>;

// =====================================================================
// 2. String Pool (문자열 풀링 시스템) 추가
// =====================================================================

// 풀링된 문자열을 나타내는 가벼운 핸들 (포인터 래퍼)
class PooledString {
    const std::string* ptr;
public:
    explicit PooledString(const std::string* p) : ptr(p) {}
    const std::string* get() const { return ptr; }
    std::string_view get_view() const { return *ptr; }

    // 포인터 기반 비교 (문자열 내용을 비교하지 않아 압도적으로 빠름)
    bool operator==(const PooledString& other) const { return ptr == other.ptr; }
};

// PooledString 전용 해시 (포인터 주소값을 그대로 해시로 사용)
struct PooledStringHasher {
    size_t operator()(const PooledString& s) const noexcept {
        // 주소값을 해싱. 시스템에 따라 std::hash<const std::string*>를 사용
        return std::hash<const std::string*>()(s.get());
    }
};

// 중앙 문자열 관리자
class StringPool {
    // std::unordered_set은 요소의 메모리 주소(포인터) 안정을 보장합니다 (Rehash가 일어나도 주소는 변하지 않음)
    std::unordered_set<std::string, StringHasher, StringEqual> pool;
public:
    PooledString intern(std::string_view sv) {
        // 이미 풀에 있는지 확인 (C++20 투명 조회로 임시 string 생성 안함)
        auto it = pool.find(sv);
        if (it != pool.end()) {
            return PooledString(&(*it));
        }
        // 없으면 새로 생성 후 삽입
        auto result = pool.insert(std::string(sv));
        return PooledString(&(*result.first));
    }
};

// =====================================================================
// 3. 컴파일 타임 문자열 생성
// =====================================================================
constexpr uint32_t cx_rand(uint32_t& state) {
    state = state * 1664525 + 1013904223;
    return state;
}

template <size_t Count, size_t Length>
struct ConstexprStringArray {
    std::array<std::array<char, Length + 1>, Count> data{};
    constexpr ConstexprStringArray(uint32_t seed) {
        const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        constexpr size_t charset_size = sizeof(charset) - 1;
        for (size_t i = 0; i < Count; ++i) {
            for (size_t j = 0; j < Length; ++j) {
                data[i][j] = charset[cx_rand(seed) % charset_size];
            }
            data[i][Length] = '\0';
        }
    }
};

constexpr size_t WORD_COUNT = 100;
constexpr size_t WORD_LENGTH = 16;
constexpr ConstexprStringArray<WORD_COUNT, WORD_LENGTH> ct_strings(12345);

template <size_t... Is>
constexpr std::array<HashedStringView, sizeof...(Is)> make_hashed_array(std::index_sequence<Is...>) {
    return { HashedStringView(std::string_view(ct_strings.data[Is].data(), WORD_LENGTH))... };
}

template <size_t... Is>
constexpr std::array<std::string_view, sizeof...(Is)> make_sv_array(std::index_sequence<Is...>) {
    return { std::string_view(ct_strings.data[Is].data(), WORD_LENGTH)... };
}

constexpr auto ct_hashed_strings = make_hashed_array(std::make_index_sequence<WORD_COUNT>{});
constexpr auto ct_string_views = make_sv_array(std::make_index_sequence<WORD_COUNT>{});

struct std_umap_hasher {
    using is_transparent = void;

    size_t operator()(const std::string_view s) const noexcept {
        return std::hash<std::string_view>{}(s);
    }
};

// =====================================================================
// 4. 벤치마크 실행
// =====================================================================
template <typename Func>
void measure(const std::string& test_name, Func&& func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms = end - start;
    std::cout << test_name << ": " << ms.count() << " ms\n";
}

int main() {
    constexpr size_t LOOKUP_COUNT = 100'000'000;

    std::unordered_map<std::string, int, std_umap_hasher, std::equal_to<>> std_map;
    StringHashTable<int> custom_map;
    std::unordered_map<PooledString, int, PooledStringHasher> pool_map;

    StringPool global_pool;
    std::vector<PooledString> interned_keys;
    interned_keys.reserve(WORD_COUNT);

    std::cout << "[데이터 세팅 중...]\n";
    for (size_t i = 0; i < WORD_COUNT; ++i) {
        std_map[std::string(ct_string_views[i])] = i;
        custom_map[std::string(ct_string_views[i])] = i;

        // 문자열을 풀에 등록하고 발급된 핸들 저장
        PooledString ps = global_pool.intern(ct_string_views[i]);
        interned_keys.push_back(ps);
        pool_map[ps] = i;
    }

    std::vector<size_t> lookup_indices;
    lookup_indices.reserve(LOOKUP_COUNT);
    std::mt19937 rng(54321);
    std::uniform_int_distribution<size_t> dist(0, WORD_COUNT - 1);
    for (size_t i = 0; i < LOOKUP_COUNT; ++i) {
        lookup_indices.push_back(dist(rng));
    }

    volatile size_t dummy = 0;

    std::cout << "\n[1억 번 랜덤 조회 벤치마크 시작]\n";
    std::cout << "---------------------------------------------------------\n";

    measure("1. std::unordered_map, std::string_view 활용", [&]() {
        size_t count = 0;
        for (size_t i : lookup_indices) {
            if (std_map.find(ct_string_views[i]) != std_map.end()) count++;
        }
        dummy = count;
        });
	std::cout << "수행 횟수: " << dummy << "\n";

    measure("2. StringHashTable (std::string_view 투명 조회 / 런타임 해싱)", [&]() {
        size_t count = 0;
        for (size_t i : lookup_indices) {
            if (custom_map.find(ct_string_views[i]) != custom_map.end()) count++;
        }
        dummy = count;
        });
    std::cout << "수행 횟수: " << dummy << "\n";

    measure("3. StringHashTable (HashedStringView / 컴파일 타임 해싱)", [&]() {
        size_t count = 0;
        for (size_t i : lookup_indices) {
            if (custom_map.find(ct_hashed_strings[i]) != custom_map.end()) count++;
        }
        dummy = count;
        });
    std::cout << "수행 횟수: " << dummy << "\n";

    measure("4. String Pool Map (PooledString / 포인터 해싱 및 비교)", [&]() {
        size_t count = 0;
        for (size_t i : lookup_indices) {
            // 이미 풀에 등록(intern)되어 발급받은 핸들을 사용한다고 가정
            if (pool_map.find(interned_keys[i]) != pool_map.end()) count++;
        }
        dummy = count;
        });
    std::cout << "수행 횟수: " << dummy << "\n";

    std::cout << "---------------------------------------------------------\n";
    std::cout << "\n[번외: 런타임 문자열을 풀에 등록 후 조회하는 전체 비용]\n";
    measure("5. 런타임 Hashed String 조회", [&]() {
        size_t count = 0;
        for (size_t i : lookup_indices) {
            // 외부 네트워크나 파일에서 문자열이 막 들어온 상황을 가정
			HashedStringView dynamic_hs(ct_string_views[i]);
            if (custom_map.find(dynamic_hs) != custom_map.end()) count++;
        }
        dummy = count;
        });
    std::cout << "수행 횟수: " << dummy << "\n";

    measure("6. 동적 intern() 호출 + Pool Map 조회", [&]() {
        size_t count = 0;
        for (size_t i : lookup_indices) {
            // 외부 네트워크나 파일에서 문자열이 막 들어온 상황을 가정
            PooledString dynamic_ps = global_pool.intern(ct_string_views[i]);
            if (pool_map.find(dynamic_ps) != pool_map.end()) count++;
        }
        dummy = count;
        });
    std::cout << "수행 횟수: " << dummy << "\n";

    return 0;
}