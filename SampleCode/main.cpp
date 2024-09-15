#include <unordered_map>
#include <iostream>

struct hash_key {
	unsigned int* a, * b;

	bool operator == (const hash_key& _other) const {
		return (a == _other.a) && (b == _other.b);
	}

	struct hasher {
		static_assert(sizeof(std::size_t) == sizeof(std::uint64_t));
		size_t operator ()(const hash_key& _key) const {
			union key_maker {
				static_assert(sizeof(size_t) == sizeof(std::uint64_t), "size_t가 64bit가 아님. 처리코드 필요");
				struct key_pair {
					std::uint32_t a, b;
				} pair;
				size_t key;
			};
			key_maker keymaker;
			keymaker.pair.a = *(_key.a);
			keymaker.pair.b = *(_key.b);
			return keymaker.key;
		}
	};
};

int main() {
	
	hash_key k;
	k.a = new unsigned int;
	k.b = new unsigned int;

	*(k.a) = 0xffffffff;
	*(k.b) = 0xffffffff;

	hash_key::hasher hs;
	size_t custom_hash = hs(k);

	std::unordered_map<hash_key, int, hash_key::hasher> kv;
	kv[k] = 5000;

	std::cout << kv[k];

	return 0;
}