#include "pch.h"
#include "Image.h"
#include "AssetLoader.h"



Image::~Image()
{
	if (m_Buffer)
	{
		free(m_Buffer);
	}
}

Image::Image(Image && image)
{
	m_Width = image.m_Width;
	m_Height = image.m_Height;
	m_Channels = image.m_Channels;
	m_Buffer = image.m_Buffer;
	image.m_Buffer = nullptr;
}

Image & Image::operator=(Image && image)
{
	m_Width = image.m_Width;
	m_Height = image.m_Height;
	m_Channels = image.m_Channels;
	m_Buffer = image.m_Buffer;
	image.m_Buffer = nullptr;

	return *this;
}
