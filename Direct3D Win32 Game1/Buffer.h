#pragma once
#include "pch.h"

class Buffer
{
public:
	Buffer();
	~Buffer();
	Buffer(const size_t size);
	Buffer(Buffer&& buffer);
	Buffer& operator=(Buffer&& buffer);
	void Resize(const size_t size);
	void Free();
	uint8* Data() { return m_Data; }
	size_t Size() { return m_Size; }
protected:
	uint8 * m_Data;
	size_t m_Size;
};

