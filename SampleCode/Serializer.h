#pragma once

#include "type_traits_Ex.h"

#include <iostream>
#include <cstdint>
#include <vector>
#include <concepts>
#include <string>


class Serializable
{
public:
	virtual bool Write() = 0;
	virtual bool Read() = 0;
};


template <typename T>
concept ValueTypes =
std::is_fundamental_v<T>;

template <typename T>
concept StringTypes =
std::is_same_v<T, std::string> ||
std::is_same_v<T, std::wstring>;

template <typename T>
concept PointerTypes = requires(T a)
{
	a->GetStrKey();
};



class BinarySerializer
{
public:
	BinarySerializer();
	~BinarySerializer();

	void Write(const unsigned char* _pSrc, size_t _size);
	size_t Read(unsigned char* _pDest, size_t _size);
	
	template <ValueTypes T>
	BinarySerializer& operator<<(const T& _data);
	template <StringTypes T>
	BinarySerializer& operator<<(const T& _data);

	template <typename T>
	BinarySerializer& operator<<(const std::vector<T>& _data);

	template <ValueTypes T>
	BinarySerializer& operator>>(T& _data);
	template <StringTypes T>
	BinarySerializer& operator>>(T& _data);

	template <typename T>
	BinarySerializer& operator>>(std::vector<T>& _data);

	
	size_t GetDataSize() const { return m_data.size(); }
	void ReserveDataSize(size_t _size) { m_data.reserve(_size); }

	size_t GetReadOffset() const { return m_ReadOffset; }
	inline size_t SetReadOffset(size_t _offset);

	size_t GetWriteOffset() const { return m_WriteOffset; }
	inline size_t SetWriteOffsert(size_t _offset);
	
	void Clear() { m_data.clear(); m_ReadOffset = (size_t)0; m_WriteOffset = (size_t)0; }
private:
	std::vector<unsigned char> m_data;
	size_t m_ReadOffset;
	size_t m_WriteOffset;
};


template<ValueTypes T>
inline BinarySerializer& BinarySerializer::operator<<(const T& _data)
{
	BinarySerializer& thisRef = *this;

	Write(reinterpret_cast<const unsigned char*>(&_data), sizeof(T));

	return thisRef;
}

template<StringTypes T>
inline BinarySerializer& BinarySerializer::operator<<(const T& _data)
{
	BinarySerializer& thisRef = *this;

	const unsigned char* src = reinterpret_cast<const unsigned char*>(_data.data());
	size_t BytesToRead = sizeof(T::value_type) * _data.size();
	
	thisRef << BytesToRead;
	Write(src, BytesToRead);

	return thisRef;
}

template<typename T>
inline BinarySerializer& BinarySerializer::operator<<(const std::vector<T>& _data)
{
	BinarySerializer& thisRef = *this;

	(*this) << _data.size();
	for (size_t i = 0; i < _data.size(); ++i)
	{
		thisRef << _data[i];
	}

	//Write(&_data, sizeof(T));

	return thisRef;
}



template<ValueTypes T>
inline BinarySerializer& BinarySerializer::operator>>(T& _data)
{
	BinarySerializer& thisRef = *this;

	Read(reinterpret_cast<unsigned char*>(&_data), sizeof(T));

	return thisRef;
}

template<StringTypes T>
inline BinarySerializer& BinarySerializer::operator>>(T& _data)
{
	BinarySerializer& thisRef = *this;

	size_t BytesToRead{};
	thisRef >> BytesToRead;
	_data.clear();
	_data.resize(BytesToRead);

	unsigned char* dest = reinterpret_cast<unsigned char*>(_data.data());
	Read(dest, BytesToRead);

	return thisRef;
}

template<typename T>
inline BinarySerializer& BinarySerializer::operator>>(std::vector<T>& _data)
{
	BinarySerializer& thisRef = *this;

	size_t size = 0;
	thisRef >> size;
	_data.resize(size);
	for (size_t i = 0; i < _data.size(); ++i)
	{
		thisRef >> _data[i];
	}

	return thisRef;
}


inline size_t BinarySerializer::SetReadOffset(size_t _offset)
{
	size_t ret = _offset;

	if (m_data.size() <= ret)
	{
		ret = m_data.size();
	}
	m_ReadOffset = ret;

	return ret;
}

inline size_t BinarySerializer::SetWriteOffsert(size_t _offset)
{
	size_t ret = _offset;

	if (m_data.size() <= ret)
	{
		ret = m_data.size();
	}
	m_WriteOffset = ret;

	return ret;
}

//#include "../json-cpp/json-forwards.h"
//class JsonSerializer
//{
//
//
//
//};
