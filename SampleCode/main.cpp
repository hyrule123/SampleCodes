#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

// 1. 컴파일 타임/런타임 하이브리드 키 (수정됨)
struct PrehashedKey {
    std::string_view str;
    size_t hash_value;

    // A. 컴파일 타임 리터럴 전용 (배열 참조가 Exact Match라 리터럴 입력 시 무조건 이것이 선택됨)
    template <size_t N>
    consteval PrehashedKey(const char(&s)[N])
        : str(s, N - 1), hash_value(hash_str(std::string_view(s, N - 1))) {}

	PrehashedKey(const std::string_view s)
		: str(s), hash_value(hash_str(s)) {}

    static constexpr size_t hash_str(std::string_view s) {
        size_t hash = 14695981039346656037ULL;
        for (char c : s) {
            hash ^= static_cast<size_t>(c);
            hash *= 1099511628211ULL;
        }
        return hash;
    }
};

// ---------------------------------------------------------
// 2. 투명한(Transparent) 해시 펑터 (모호성 완벽 제거 버전)
// ---------------------------------------------------------

struct TransparentHash {
    using is_transparent = void;

    // std::string 정확한 매칭 (변환 없이 바로 해싱)
    size_t operator()(const std::string& s) const { return PrehashedKey::hash_str(s); }

    // 기존 매칭들
    size_t operator()(std::string_view s) const { return PrehashedKey::hash_str(s); }
    size_t operator()(const PrehashedKey& pk) const { return pk.hash_value; }
};

struct TransparentEqual {
    using is_transparent = void;

    // 2. std::string_view 그룹
    bool operator()(std::string_view lhs, std::string_view rhs) const { return lhs == rhs; }
    bool operator()(std::string_view lhs, const PrehashedKey& rhs) const { return lhs == rhs.str; }
    bool operator()(const PrehashedKey& lhs, std::string_view rhs) const { return lhs.str == rhs; }

    // 3. PrehashedKey 전용 그룹
    bool operator()(const PrehashedKey& lhs, const PrehashedKey& rhs) const {
        return lhs.hash_value == rhs.hash_value && lhs.str == rhs.str;
    }
};

// 3. 완전히 정리된 래퍼 컨테이너 (코드가 절반으로 줄었습니다!)
template <typename ValueType>
class HybridStringMap {
private:
    using MapType = std::unordered_map<std::string, ValueType, TransparentHash, TransparentEqual>;
    MapType map_;

public:
    // [단일 operator[]] 
    // 리터럴을 넣으면 컴파일 타임 해싱, std::string을 넣으면 런타임 해싱이 자동으로 분기됩니다.
    ValueType& operator[](const PrehashedKey& key) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            return it->second; // 이미 있으면 즉시 반환 (비용 0)
        }

        // 못 찾았을 때만 key.str(string_view)를 이용해 런타임에 메모리 할당 후 삽입
        return map_[std::string(key.str)];
    }

    // [단일 find]
    ValueType* find(const PrehashedKey& key) {
        auto it = map_.find(key);
        return it != map_.end() ? &it->second : nullptr;
    }

    void clear() { map_.clear(); }
    size_t size() const { return map_.size(); }
};

// 4. 테스트 
int main() {
    HybridStringMap<int> GameData;

    // ⭕️ 완벽하게 consteval로 작동합니다! (함수 외부에서 변환되므로 에러 없음)
    GameData["PlayerHp"] = 100;

	int t = GameData["PlayerHp"]; // consteval 탐색

    std::string dynamicKey = "MonsterHp";
    // ⭕️ 런타임 해싱으로 자연스럽게 작동합니다!
    GameData[std::string_view(dynamicKey)] = 200;

    if (int* hp = GameData.find("PlayerHp")) { // consteval 탐색
        std::cout << "Player HP: " << *hp << "\n";
    }

    return 0;
}