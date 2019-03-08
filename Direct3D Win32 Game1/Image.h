#pragma once

class Image
{
public:
	Image() :
		m_Width(0), m_Height(0), m_Channels(0), m_Buffer(0) {}
	Image(size_t w, size_t h, size_t c, uint8* d) :
		m_Width(w), m_Height(h), m_Channels(c), m_Buffer(d) {}
	~Image();
	Image(Image&& image);
	Image& operator=(Image&& image);
	size_t Width()    { return m_Width; }
	size_t Height()   { return m_Height; }
	size_t Channels() { return m_Channels; }
	uint8* Data()     { return m_Buffer; }
	bool Empty() { return m_Buffer == NULL; }
protected:
	size_t m_Width;
	size_t m_Height;
	size_t m_Channels;
	uint8* m_Buffer;
};

