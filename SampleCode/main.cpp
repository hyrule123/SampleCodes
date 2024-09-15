#include <unordered_map>
#include <iostream>

struct custom {
	~custom() {
		std::cout << "DESTRUCTOR CALLED'\n";
	}

	int i;
};

struct hash_key {
	unsigned int* a, * b;

	bool operator == (const hash_key& _other) const {
		return (a == _other.a) && (b == _other.b);
	}

	struct hasher {
		static_assert(sizeof(std::size_t) == sizeof(std::uint64_t));
		size_t operator ()(const hash_key& _key) const {
			size_t ret{};
			ret = *(_key.a);
			ret <<= 32;
			ret |= *(_key.b);
			return ret;
		}

		custom operator()(const custom& c) const {
			custom ret{};
			ret.i = c.i;
			return ret;
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

	custom inp{ 3 };
	custom outp = hs(inp);

	std::unordered_map<hash_key, int, hash_key::hasher> kv;
	kv[k] = 5000;

	std::cout << kv[k];

	return 0;
}