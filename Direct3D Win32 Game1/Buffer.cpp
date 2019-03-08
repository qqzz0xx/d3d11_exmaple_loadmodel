#include "pch.h"
#include "Buffer.h"



Buffer::Buffer()
{
	m_Data = nullptr;
	m_Size = 0;
}

Buffer::Buffer(const size_t size)
{
	m_Data = new uint8[size];
	m_Size = size;
}

Buffer::Buffer(Buffer && buffer)
{
	m_Data = buffer.m_Data;
	m_Size = buffer.m_Size;
	buffer.m_Data = nullptr;
}

Buffer & Buffer::operator=(Buffer && buffer)
{
	m_Size = buffer.m_Size;
	m_Data = buffer.m_Data;
	buffer.m_Data = nullptr;
	return *this;
}

Buffer::~Buffer()
{
	Free();
}

void Buffer::Resize(const size_t size)
{
	Free();

	m_Data = new uint8[size];
	m_Size = size;
}

void Buffer::Free()
{
	if (m_Data)
	{
		delete m_Data;
		m_Data = NULL;
		m_Size = 0;
	}
}

