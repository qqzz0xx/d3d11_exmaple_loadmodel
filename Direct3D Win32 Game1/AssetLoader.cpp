#include "pch.h"
#include "AssetLoader.h"
#include <sstream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::set<std::string> AssetLoader::m_SearchPaths;

void AssetLoader::AddSearchPath(const std::string & path)
{
	m_SearchPaths.emplace(path);
}

FILE * AssetLoader::FileOpen(const std::string & fileName, FileMode fileMode)
{
	FILE* fp = nullptr;
	for (const auto& var : m_SearchPaths)
	{
		std::stringstream ss;
		ss << var << "/" << fileName;
		if (fileMode == OPEN_MODE_TEXT)
		{
			fp = fopen(ss.str().c_str(), "r");
		}
		else
		{
			fp = fopen(ss.str().c_str(), "rb");
		}

		if (fp != nullptr)
		{
			return fp;
		}
	}
	return nullptr;
}


bool AssetLoader::FileExist(const std::string & fileName)
{
	auto fp = FileOpen(fileName);
	if (fp != nullptr)
	{
		fclose(fp);
		return true;
	}
	return false;
}

Buffer AssetLoader::LoadBytes(const std::string & fileName)
{
	auto fp = FileOpen(fileName, OPEN_MODE_BIT);
	Buffer buffer;
	if (fp)
	{
		auto size = GetFileSize(fp);
		buffer.Resize(size);
		auto pData = buffer.Data();
		fread(pData, sizeof(pData[0]), size, fp);
		fclose(fp);
	}
	return std::move(buffer);
}

Image AssetLoader::LoadImage(const std::string & fileName)
{
	for (const auto& var : m_SearchPaths)
	{
		std::stringstream ss;
		ss << var << "/" << fileName;

		int width, height, nrChannels;
		unsigned char *data = stbi_load(ss.str().c_str(), &width, &height, &nrChannels, 0);

		return Image(width, height, nrChannels, data);
	}

	return Image();
}

std::string AssetLoader::GetFullPath(const std::string & fileName)
{
	for (const auto& var : m_SearchPaths)
	{
		std::stringstream ss;
		ss << var << "/" << fileName;

		auto fp = fopen(ss.str().c_str(), "rb");

		if (fp != nullptr)
		{
			return ss.str();
		}
	}

	return nullptr;
}

size_t AssetLoader::GetFileSize(FILE * fp)
{
	auto pos = ftell(fp);
	fseek(fp, 0, SEEK_END);
	size_t length = ftell(fp);
	fseek(fp, pos, SEEK_SET);
	return length;
}
