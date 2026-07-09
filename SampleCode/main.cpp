#include <iostream>

#include <string>
#include <string_view>
#include <cstdint>

#include <iostream>
#include <unordered_map>
#include <unordered_set>

// ---------------------------------------------------------
// 1. 런타임 문자열 저장소 (String Pool)
// ---------------------------------------------------------
class StringPool {
public:
    static const std::string_view intern(const std::string& str) {
        // 싱글톤 패턴의 전역 문자열 풀
        static std::unordered_set<std::string> pool;

        // set에 문자열을 삽입. 이미 존재하면 기존 것을 반환
        auto it = pool.insert(str).first;

        // C++ 표준 보장: unordered_set 내부의 요소는 삭제되지 않는 한 포인터가 무효화되지 않음
        return std::string_view(*it);
    }
};

// ---------------------------------------------------------
// 2. 하이브리드 StringId 구조체
// ---------------------------------------------------------
struct StringId {
    size_t hash_value;
    const std::string_view str_ptr; // 문자열 원본을 가리키는 포인터

    // A. 컴파일 타임 생성자 (상수 문자열 리터럴 전용)
    template <size_t N>
    consteval StringId(const char(&str)[N])
        : hash_value(fnv1a_hash(str, N - 1))
        , str_ptr(str) {} // 데이터 영역(.rdata)의 리터럴 주소를 그대로 가리킴

    // B. 런타임 생성자 (동적 문자열 전용)
    explicit StringId(const std::string& str)
        : hash_value(fnv1a_hash_runtime(str.c_str(), str.length()))
        , str_ptr(StringPool::intern(str)) {} // 풀에 저장하고 그 주소를 가리킴

    // C. 비교 연산자 (Map 내부 탐색용)
    bool operator==(const StringId& other) const {
        // 1차: 해시값 비교 (엄청나게 빠름)
        if (hash_value != other.hash_value) return false;
        // 2차: 포인터가 같으면 무조건 같은 문자열
        if (str_ptr == other.str_ptr) return true;
        // 3차: 극히 희박한 해시 충돌 방지를 위한 최후의 문자열 비교
        return str_ptr == other.str_ptr;
    }

private:
    // 컴파일 타임 해시 함수
    static consteval size_t fnv1a_hash(const char* str, size_t length) {
        size_t hash = 14695981039346656037ULL;
        for (size_t i = 0; i < length; ++i) {
            hash ^= static_cast<size_t>(str[i]);
            hash *= 1099511628211ULL;
        }
        return hash;
    }

    // 런타임 해시 함수 (로직은 동일하나 consteval 제약이 없음)
    static size_t fnv1a_hash_runtime(const char* str, size_t length) {
        size_t hash = 14695981039346656037ULL;
        for (size_t i = 0; i < length; ++i) {
            hash ^= static_cast<size_t>(str[i]);
            hash *= 1099511628211ULL;
        }
        return hash;
    }
};

// ---------------------------------------------------------
// 3. std::unordered_map 지원을 위한 std::hash 특수화
// ---------------------------------------------------------
template<>
struct std::hash<StringId> {
    size_t operator()(const StringId& id) const {
        return id.hash_value; // 이미 계산된 해시를 그대로 반환 (O(1))
    }
};

int main()
{

    std::unordered_map<StringId, int> myMap;

    myMap["Test"] = 3;

    std::string tst = "Test";

    int t = myMap["Test"];

    int tt = myMap[StringId(tst)];


    return 0;
}