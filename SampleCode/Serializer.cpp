#include "Serializer.h"

BinarySerializer::BinarySerializer()
	: m_data()
	, m_ReadOffset()
	, m_WriteOffset()
{
}

BinarySerializer::~BinarySerializer()
{
}

void BinarySerializer::Write(const unsigned char* _pSrc, size_t _size)
{
	if (nullptr == _pSrc || 0 == _size)
	{
		return;
	}

	size_t reqSize = m_WriteOffset + _size;
	if (m_data.size() < reqSize)
	{
		if (m_data.capacity() < reqSize)
		{
			m_data.reserve(reqSize * 2);
		}

		m_data.resize(reqSize);
	}

	unsigned char* dest = m_data.data() + m_WriteOffset;
	memcpy_s(dest, m_data.size() - m_WriteOffset, _pSrc, _size);
	m_WriteOffset = reqSize;

}

size_t BinarySerializer::Read(unsigned char* _pDest, size_t _size)
{
	if (nullptr == _pDest || 0 == _size)
	{
		return 0;
	}
	//이미 끝까지 읽은 경우
	else if (m_data.size() <= m_ReadOffset)
	{
		return 0;
	}
	//읽고자 하는 바이트 수가 최대 사이즈를 넘어설 경우 최대 사이즈까지만 읽음
	else if (m_data.size() < m_ReadOffset + _size)
	{
		_size = m_data.size() - m_ReadOffset;
	}

	unsigned char* src = m_data.data() + m_ReadOffset;
	memcpy_s(_pDest, _size, src, _size);
	m_ReadOffset += _size;

	return _size;
}


