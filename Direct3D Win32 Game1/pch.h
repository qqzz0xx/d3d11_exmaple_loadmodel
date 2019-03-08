//
// pch.h
// Header for standard system include files.
//

#pragma once

#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0601
#include <SDKDDKVer.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <wrl/client.h>

#include <d3d11_1.h>
#include <dxgi1_2.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>
#include <vector>
#include <set>

namespace DX
{
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            // Set a breakpoint on this line to catch DirectX API errors
            throw std::exception();
        }
    }
}

// type
typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned int		uint32;
#ifdef _WIN32
typedef unsigned __int64	uint64;
#else
typedef unsigned long long	uint64;
#endif
typedef signed char			int8;
typedef signed short		int16;
typedef signed int			int32;
#ifdef _WIN32
typedef signed __int64		int64;
#else
typedef long long			int64;
#endif
typedef unsigned char		uchar;
typedef unsigned short		ushort;
typedef unsigned int		uint;
typedef unsigned long		ulong;
